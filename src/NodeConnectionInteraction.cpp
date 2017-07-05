#include "NodeConnectionInteraction.hpp"

#include "ConnectionGraphicsObject.hpp"
#include "NodeGraphicsObject.hpp"
#include "NodeDataModel.hpp"
#include "DataModelRegistry.hpp"
#include "FlowScene.hpp"

using QtNodes::NodeConnectionInteraction;
using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::FlowScene;
using QtNodes::Node;
using QtNodes::Connection;
using QtNodes::NodeDataModel;


NodeConnectionInteraction::
NodeConnectionInteraction(Node& node, Connection& connection, FlowScene& scene)
  : _node(&node)
  , _connection(&connection)
  , _scene(&scene)
{}


bool
NodeConnectionInteraction::
canConnect(PortIndex &portIndex, bool& typeConversionNeeded, std::unique_ptr<NodeDataModel> & converterModel) const
{
  typeConversionNeeded = false;

  // 1) Connection requires a port

  PortType requiredPort = connectionRequiredPort();

  if (requiredPort == PortType::None)
  {
    return false;
  }

  // 2) connection point is on top of the node port

  QPointF connectionPoint = connectionEndScenePosition(requiredPort);

  portIndex = nodePortIndexUnderScenePoint(requiredPort,
                                           connectionPoint);

  if (portIndex == INVALID)
  {
    return false;
  }

  // 3) Node port is vacant

  // port should be empty
  if (!nodePortIsEmpty(requiredPort, portIndex))
    return false;

  // 4) Connection type equals node port type, or there is a registered type conversion that can translate between the two

  auto connectionDataType = _connection->dataType();

  auto const   &modelTarget = _node->nodeDataModel();
  NodeDataType candidateNodeDataType = modelTarget->dataType(requiredPort, portIndex);

  if (connectionDataType.id != candidateNodeDataType.id)
  {
    if (requiredPort == PortType::In)
    {
      return typeConversionNeeded = (converterModel = _scene->registry().getTypeConverter(connectionDataType.id, candidateNodeDataType.id)) != nullptr;
    }
    return typeConversionNeeded = (converterModel = _scene->registry().getTypeConverter(candidateNodeDataType.id, connectionDataType.id)) != nullptr;
  }

  if (_connection->getNode(PortType::In))
  {
    if (!_node->nodeDataModel()->canConnect(PortType::In,
                                            _connection->getNode(PortType::In)->nodeDataModel(),
                                            connectionDataType) ||
        !_connection->getNode(PortType::In)->nodeDataModel()->canConnect(PortType::Out,
                                                                         _node->nodeDataModel(),
                                                                         connectionDataType))
    {
      return false;
    }
  }

  if (_connection->getNode(PortType::Out))
  {
    if (!_node->nodeDataModel()->canConnect(PortType::Out,
                                            _connection->getNode(PortType::Out)->nodeDataModel(),
                                            connectionDataType) ||
        !_connection->getNode(PortType::Out)->nodeDataModel()->canConnect(PortType::In,
                                                                          _node->nodeDataModel(),
                                                                          connectionDataType))
    {
      return false;
    }
  }

  return true;
}


bool
NodeConnectionInteraction::
tryConnect() const
{
  // 1) Check conditions from 'canConnect'
  PortIndex portIndex = INVALID;
  bool typeConversionNeeded = false; 
  std::unique_ptr<NodeDataModel> typeConverterModel;

  if (!canConnect(portIndex, typeConversionNeeded, typeConverterModel))
  {
    return false;
  }

  // 2) Assign node to required port in Connection

  PortType requiredPort = connectionRequiredPort();
  _node->nodeState().setConnection(requiredPort,
                                   portIndex,
                                   *_connection);

  // 3) Assign Connection to empty port in NodeState
  // The port is not longer required after this function
  _connection->setNodeToPort(*_node, requiredPort, portIndex);

  // 4) Adjust Connection geometry

  _node->nodeGraphicsObject().moveConnections();

  // 5) Poke model to intiate data transfer

  auto outNode = _connection->getNode(PortType::Out);
  if (outNode)
  {
    PortIndex outPortIndex = _connection->getPortIndex(PortType::Out);
    outNode->onDataUpdated(outPortIndex);
  }

  return true;
}


/// 1) Node and Connection should be already connected
/// 2) If so, clear Connection entry in the NodeState
/// 3) Set Connection end to 'requiring a port'
bool
NodeConnectionInteraction::
disconnect(PortType portToDisconnect) const
{
  PortIndex portIndex =
    _connection->getPortIndex(portToDisconnect);

  NodeState &state = _node->nodeState();

  // clear pointer to Connection in the NodeState
  state.getEntries(portToDisconnect)[portIndex].clear();

  // 4) Propagate invalid data to IN node
  _connection->propagateEmptyData();

  // clear Connection side
  _connection->clearNode(portToDisconnect);

  _connection->setRequiredPort(portToDisconnect);

  _connection->getConnectionGraphicsObject().grabMouse();

  return true;
}


// ------------------ util functions below

PortType
NodeConnectionInteraction::
connectionRequiredPort() const
{
  auto const &state = _connection->connectionState();

  return state.requiredPort();
}


QPointF
NodeConnectionInteraction::
connectionEndScenePosition(PortType portType) const
{
  auto &go =
    _connection->getConnectionGraphicsObject();

  ConnectionGeometry& geometry = _connection->connectionGeometry();

  QPointF endPoint = geometry.getEndPoint(portType);

  return go.mapToScene(endPoint);
}


QPointF
NodeConnectionInteraction::
nodePortScenePosition(PortType portType, PortIndex portIndex) const
{
  NodeGeometry const &geom = _node->nodeGeometry();

  QPointF p = geom.portScenePosition(portIndex, portType);

  NodeGraphicsObject& ngo = _node->nodeGraphicsObject();

  return ngo.sceneTransform().map(p);
}


PortIndex
NodeConnectionInteraction::
nodePortIndexUnderScenePoint(PortType portType,
                             QPointF const & scenePoint) const
{
  NodeGeometry const &nodeGeom = _node->nodeGeometry();

  QTransform sceneTransform =
    _node->nodeGraphicsObject().sceneTransform();

  PortIndex portIndex = nodeGeom.checkHitScenePoint(portType,
                                                    scenePoint,
                                                    sceneTransform);
  return portIndex;
}


bool
NodeConnectionInteraction::
nodePortIsEmpty(PortType portType, PortIndex portIndex) const
{
  NodeState const & nodeState = _node->nodeState();

  auto const & entries = nodeState.getEntries(portType);

  if (entries[portIndex].empty()) return true;

  const auto inPolicy = _node->nodeDataModel()->portInConnectionPolicy(portIndex);
  const auto outPolicy = _node->nodeDataModel()->portOutConnectionPolicy(portIndex);
  return (portType == PortType::Out && outPolicy == NodeDataModel::ConnectionPolicy::Many) ||
         (portType == PortType::In && inPolicy == NodeDataModel::ConnectionPolicy::Many);
}

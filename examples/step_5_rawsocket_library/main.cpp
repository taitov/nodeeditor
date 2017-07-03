#include <tvm/gui.h>
#include <tvm/library/base.h>
#include <tvm/library/console.h>
#include <tvm/library/timer.h>
#include <tvm/library/rawsocket.h>

#include "virtualmachine.h"

using namespace nVirtualMachine;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	cVirtualMachine virtualMachine;

	if (!registerVirtualMachine(virtualMachine))
	{
		return 1;
	}

	nGui::cIde ide(&virtualMachine, "tfvm:step_4_base_library");

	return app.exec();
}

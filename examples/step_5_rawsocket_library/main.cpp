#include <tfvm/gui.h>
#include <tfvm/library/base.h>
#include <tfvm/library/console.h>
#include <tfvm/library/timer.h>
#include <tfvm/library/rawsocket.h>

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

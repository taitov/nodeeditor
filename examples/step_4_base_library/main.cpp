#include <tfvm/gui.h>
#include <tfvm/library/base.h>
#include <tfvm/library/console.h>

using namespace nVirtualMachine;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	cVirtualMachine virtualMachine;

	if (!virtualMachine.registerLibraries(new nLibrary::cBase(),
	                                      new nLibrary::cConsole()))
	{
		return 1;
	}

	nGui::cIde ide(&virtualMachine, "tfvm:step_4_base_library");

	return app.exec();
}

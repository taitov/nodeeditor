#include <tvm/gui.h>

#include "example.h"

using namespace nVirtualMachine;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	cVirtualMachine virtualMachine;

	if (!virtualMachine.registerLibraries(new nLibrary::cExample()))
	{
		return 1;
	}

	nGui::cIde ide(&virtualMachine, "tfvm:helloworld");

	return app.exec();
}

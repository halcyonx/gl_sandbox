#include <Application.h>
#include "application/SandboxApp.h"

int main(int argc, char* argv[])
{
	Application app;
	RunApplication(app, new SandboxApp);
	return 0;
}

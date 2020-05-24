#include <Application.h>
#include <memory>

int main(int argc, char* argv[])
{
	auto app = std::make_unique<Application>();
	app->Start();
	return 0;
}

#include <Application.hpp>

using namespace std;

int main(int argc, char** argv)
{
	SylvonixNative::WindowDescriptor windowDescriptor{
		.title = "TestEngine",
		.width = 1280,
		.height = 720
	};

	SylvonixNative::ApplicationDescriptor appDescriptor{
		.name = "TestEngine",
		.version = {0, 0, 0},
		.windowDescriptor = windowDescriptor
	};

	SylvonixNative::Application app(appDescriptor, argc, argv);

	app.startMainLoop();

	return 0;
}

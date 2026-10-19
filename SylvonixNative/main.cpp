#include <Application.hpp>

using namespace std;

int main()
{
	SylvonixNative::WindowDescriptor windowDescriptor{
		.title = "TestEngine",
		.width = 1280,
		.height = 720
	};

	SylvonixNative::ApplicationDescriptor appDescriptor{
		.name = "TestEngine",
		.version = "0.1.0",
		.windowDescriptor = windowDescriptor
	};

	SylvonixNative::Application app(appDescriptor);

	app.startMainLoop();

	return 0;
}

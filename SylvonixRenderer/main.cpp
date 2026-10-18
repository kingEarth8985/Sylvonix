#include <Application.hpp>

using namespace std;

int main()
{
	SylvonixRenderer::WindowDescriptor windowDescriptor{
		.title = "TestEngine",
		.width = 1280,
		.height = 720
	};

	SylvonixRenderer::ApplicationDescriptor appDescriptor{
		.name = "TestEngine",
		.version = "0.1.0",
		.windowDescriptor = windowDescriptor
	};

	SylvonixRenderer::Application app(appDescriptor);
}

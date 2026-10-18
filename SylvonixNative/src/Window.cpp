#include <Window.hpp>
#include <DebugChecks.hpp>

#include <cassert>

namespace SylvonixNative
{
	Window::Window(WindowDescriptor descriptor, WindowVulkanObjects objects)
	{
		instance = objects.instance;
		SDL_Window* window = SDL_CreateWindow(descriptor.title.c_str(), descriptor.width, descriptor.height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
		assert(window);
		chk(SDL_Vulkan_CreateSurface(window, objects.instance, nullptr, &surface));
		chk(SDL_GetWindowSize(window, &descriptor.width, &descriptor.height));
	}

	Window::~Window()
	{
		if (surface)
		{
			vkDestroySurfaceKHR(
				instance,
				surface,
				nullptr
			);

			surface = VK_NULL_HANDLE;
		}

		if (window)
		{
			SDL_DestroyWindow(window);

			window = nullptr;
		}
	}
}
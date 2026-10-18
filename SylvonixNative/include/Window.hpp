#pragma once

#include <VulkanCommon.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <Descriptors.hpp>

namespace SylvonixNative
{

	class Device;

	struct WindowVulkanObjects
	{
		Device* device;
		VkInstance instance;
	};

	class Window
	{
	public:

		Window(WindowDescriptor descriptor, WindowVulkanObjects objects);
		~Window();

		VkSurfaceKHR getSurface() { return surface; }
		WindowDescriptor* getDescriptor() { return &descriptor; }

	private:
		VkInstance instance{ VK_NULL_HANDLE };

		SDL_Window* window;
		VkSurfaceKHR surface{ VK_NULL_HANDLE };

		WindowDescriptor descriptor;

	};
}
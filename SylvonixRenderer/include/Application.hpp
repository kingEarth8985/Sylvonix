#pragma once

#include <string>
#include <memory>

#include <VulkanCommon.hpp>
#include <Descriptors.hpp>

namespace SylvonixRenderer
{

	class Device;
	class Window;
	class SwapChain;

	class Application
	{
	public:
		Application(const ApplicationDescriptor& descriptor);

		~Application();

	private:
		VkInstance instance{ VK_NULL_HANDLE };
		VmaAllocator allocator{ VK_NULL_HANDLE };

		std::unique_ptr<Device> device;
		std::unique_ptr<Window> window;
		std::unique_ptr<SwapChain> swapChain;
	};
}
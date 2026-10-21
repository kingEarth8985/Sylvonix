#pragma once

#include <string>
#include <memory>

#include <VulkanCommon.hpp>
#include <Descriptors.hpp>
#include <Shader.hpp>

namespace SylvonixNative
{

	class Device;
	class Window;
	class SwapChain;

	class Application
	{
	public:
		Application(const ApplicationDescriptor& descriptorm, int argc, char** argv);

		~Application();

		void startMainLoop();

	private:
		VkInstance instance{ VK_NULL_HANDLE };
		VmaAllocator allocator{ VK_NULL_HANDLE };

		std::unique_ptr<Device> device;
		std::unique_ptr<Window> window;
		std::unique_ptr<SwapChain> swapChain;
	};
}
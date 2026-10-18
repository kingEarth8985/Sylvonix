#include <Application.hpp>
#include <DebugChecks.hpp>
#include <Device.hpp>
#include <Window.hpp>
#include <SwapChain.hpp>

namespace SylvonixRenderer
{
	VmaAllocator CreateVMAAllocator(
		Device& device,
		VkInstance instance)
	{
		VmaAllocator allocator;

		VmaVulkanFunctions vkFunctions{
			.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
			.vkGetDeviceProcAddr = vkGetDeviceProcAddr,
			.vkCreateImage = vkCreateImage
		};

		VmaAllocatorCreateInfo allocatorCI{
			.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
			.physicalDevice = device.getPhysicalDevice(),
			.device = device.getDevice(),
			.pVulkanFunctions = &vkFunctions,
			.instance = instance
		};
		chk(vmaCreateAllocator(&allocatorCI, &allocator));

		return allocator;
	}

	Application::Application(const ApplicationDescriptor& descriptor)
	{
		chk(SDL_Init(SDL_INIT_VIDEO));
		chk(SDL_Vulkan_LoadLibrary(NULL));
		volkInitialize();

		VkApplicationInfo appInfo{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = descriptor.name.c_str(),
			.apiVersion = VK_API_VERSION_1_3,
		};
		uint32_t instanceExtensionsCount{ 0 };
		char const* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };
		VkInstanceCreateInfo instanceCI{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &appInfo,
			.enabledExtensionCount = instanceExtensionsCount,
			.ppEnabledExtensionNames = instanceExtensions,
		};
		chk(vkCreateInstance(&instanceCI, nullptr, &instance));
		volkLoadInstance(instance);

		device = std::make_unique<Device>(instance);

		allocator = CreateVMAAllocator(*device, instance);

		WindowVulkanObjects windowVKObjects{
			.device = device.get(),
			.instance = instance,
		};
		window = std::make_unique<Window>(descriptor.windowDescriptor, windowVKObjects);
		SwapChainVulkanObjects swapVKObjects{
			.device = device.get(),
			.window = window.get(),
		};
		swapChain = std::make_unique<SwapChain>(swapVKObjects);
	}

	Application::~Application()
	{
		
	}
}
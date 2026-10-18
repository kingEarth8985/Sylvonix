#include <Application.hpp>
#include <DebugChecks.hpp>
#include <Device.hpp>
#include <Window.hpp>
#include <SwapChain.hpp>

namespace SylvonixNative
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
		
		const char* validationLayers[] =
		{
			"VK_LAYER_KHRONOS_validation"
		};
		
		VkInstanceCreateInfo instanceCI{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = 1,
			.ppEnabledLayerNames = validationLayers,
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
			.allocator = allocator,
		};
		swapChain = std::make_unique<SwapChain>(swapVKObjects);
	}

	Application::~Application()
	{
		//vkDeviceWaitIdle(device->getDevice());
		std::cout << "Destructing SwapChain\n";
		swapChain.reset();

		std::cout << "Destructing Window\n";
		window.reset();

		std::cout << "Destructing Device\n";
		device.reset();

		if (allocator)
		{
			std::cout << "Destructing Allocator\n";
			vmaDestroyAllocator(allocator);
			allocator = VK_NULL_HANDLE;
		}

		if (instance)
		{
			std::cout << "Destructing Instance\n";
			vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;
		}
		
		SDL_Quit();

		std::cout << "Exited Without Errors \n";
	}
}
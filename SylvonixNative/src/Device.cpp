#include <Device.hpp>
#include <DebugChecks.hpp>
#include <vector>
#include <iostream>
#include <SDL3/SDL_vulkan.h>

namespace SylvonixNative
{

	VkPhysicalDevice PickDevice(
		const std::vector<VkPhysicalDevice>& fallbackDevices,
		const std::vector<VkPhysicalDevice>& dedicatedDevices)
	{
		if (!dedicatedDevices.empty())
		{
			return dedicatedDevices[0];
		}
		else if (!fallbackDevices.empty())
		{
			return fallbackDevices[0];
		}
		else
		{
			std::cerr << "No suitable Vulkan device found. "
				"I would recommend acquiring a GPU.\n";
			return VK_NULL_HANDLE;
		}
	}

	Device::Device(VkInstance instance)
	{
		uint32_t deviceCount{ 0 };
		chk(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
		std::vector<VkPhysicalDevice> devices(deviceCount);
		chk(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()));

		std::vector<VkPhysicalDevice> fallbackDevices;
		std::vector<VkPhysicalDevice> dedicatedDevices;

		for (uint32_t i = 0; i < deviceCount; ++i)
		{
			VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
			vkGetPhysicalDeviceProperties2(devices[i], &deviceProperties);
			//std::cout << "Possible device: " << deviceProperties.properties.deviceName << "\n"; //we could use this later if we want to give access to debug info
			if (deviceProperties.properties.deviceType ==
				VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				dedicatedDevices.push_back(devices[i]);
			}
			else
			{
				fallbackDevices.push_back(devices[i]);
			}
		}

		physicalDevice = PickDevice(fallbackDevices, dedicatedDevices);

		VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
		vkGetPhysicalDeviceProperties2(physicalDevice, &deviceProperties);
		std::cout << "Selected device: " << deviceProperties.properties.deviceName << "\n";

		// Find a queue family for graphics
		uint32_t queueFamilyCount{ 0 };
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		uint32_t queueFamily{ 0 };
		for (size_t i = 0; i < queueFamilies.size(); i++) {
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				queueFamily = i;
				break;
			}
		}
		chk(SDL_Vulkan_GetPresentationSupport(instance, physicalDevice, queueFamily));

		// Logical device
		const float qfpriorities{ 1.0f };
		VkDeviceQueueCreateInfo queueCI{ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .queueFamilyIndex = queueFamily, .queueCount = 1, .pQueuePriorities = &qfpriorities };
		VkPhysicalDeviceVulkan12Features enabledVk12Features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .descriptorIndexing = true, .shaderSampledImageArrayNonUniformIndexing = true, .descriptorBindingVariableDescriptorCount = true, .runtimeDescriptorArray = true, .bufferDeviceAddress = true };
		VkPhysicalDeviceVulkan13Features enabledVk13Features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, .pNext = &enabledVk12Features, .synchronization2 = true, .dynamicRendering = true };
		VkPhysicalDeviceFeatures enabledVk10Features{ .samplerAnisotropy = VK_TRUE };
		const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkDeviceCreateInfo deviceCI{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &enabledVk13Features,
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queueCI,
			.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
			.ppEnabledExtensionNames = deviceExtensions.data(),
			.pEnabledFeatures = &enabledVk10Features
		};
		chk(vkCreateDevice(physicalDevice, &deviceCI, nullptr, &device));
		vkGetDeviceQueue(device, queueFamily, 0, &queue);
	}

	Device::~Device()
	{
		if (device)
		{
			vkDeviceWaitIdle(device);

			vkDestroyDevice(device, nullptr);

			device = VK_NULL_HANDLE;
		}
	}
}
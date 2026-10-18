#pragma once
#include <VulkanCommon.hpp>

namespace SylvonixRenderer
{
	class Device
	{
	public:
		Device(VkInstance instance);
		~Device();

		VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
		VkDevice getDevice() { return device; }
		VkQueue getQueue() { return queue; }

	private:
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue queue;

	};
}
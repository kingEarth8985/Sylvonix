#pragma once
#include <vulkan/vulkan.h>
#include <iostream>

static inline void chk(VkResult result) {
	if (result != VK_SUCCESS) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(static_cast<int>(result));
	}
}

static inline void chk(bool result) {
	if (!result) {
		std::cerr << "Call returned an error\n";
		exit(1);
	}
}
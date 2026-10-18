#pragma once
#include <VulkanCommon.hpp>
#include <vector>

namespace SylvonixRenderer
{
	class Device;
	class Window;

	struct SwapChainVulkanObjects
	{
		Device* device;
		Window* window;
	};

	class SwapChain
	{
	public:
		SwapChain(SwapChainVulkanObjects objects);

		~SwapChain();

	private:

		VkSurfaceCapabilitiesKHR surfaceCaps{};

		VkExtent2D swapchainExtent{};

		VkSwapchainKHR swapchain{};

		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		VkFormat imageFormat;

	};
}
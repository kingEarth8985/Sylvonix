#pragma once
#include <VulkanCommon.hpp>
#include <vector>
#include <memory>

namespace SylvonixNative
{
	class Device;
	class Window;

	class Image;

	struct SwapChainVulkanObjects
	{
		Device* device;
		Window* window;
		VmaAllocator allocator;
	};

	class SwapChain
	{
	public:
		SwapChain(SwapChainVulkanObjects objects);

		~SwapChain();

		Image* GetDepthImage() const { return depthImage.get(); }

        Image* GetHDRImage() const { return hdrImage.get(); }

		Image* GetSDRImage() const { return sdrImage.get(); }

	private:
		Device* device{};
		
		VkSurfaceCapabilitiesKHR surfaceCaps{};

		VkExtent2D swapchainExtent{};

		VkSwapchainKHR swapchain{};

		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		VkFormat imageFormat{};

		std::unique_ptr<Image> depthImage;
    	std::unique_ptr<Image> hdrImage;
		std::unique_ptr<Image> sdrImage;

	};
}
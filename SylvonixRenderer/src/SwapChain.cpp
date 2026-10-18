#include <SwapChain.hpp>
#include <DebugChecks.hpp>
#include <Device.hpp>
#include <Window.hpp>

namespace SylvonixRenderer
{
	SwapChain::SwapChain(SwapChainVulkanObjects objects)
	{
		auto surface = objects.window->getSurface();
		auto device = objects.device->getDevice();
		VkSurfaceCapabilitiesKHR surfaceCaps{};
		chk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(objects.device->getPhysicalDevice(), surface, &surfaceCaps));
		VkExtent2D swapchainExtent{ surfaceCaps.currentExtent };
		if (surfaceCaps.currentExtent.width == 0xFFFFFFFF) {
			swapchainExtent = { .width = static_cast<uint32_t>(objects.window->getDescriptor()->width), .height = static_cast<uint32_t>(objects.window->getDescriptor()->height)};
		}

		const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_SRGB };
		VkSwapchainCreateInfoKHR swapchainCI{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface,
			.minImageCount = surfaceCaps.minImageCount,
			.imageFormat = imageFormat,
			.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
			.imageExtent{.width = swapchainExtent.width, .height = swapchainExtent.height },
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
			.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = VK_PRESENT_MODE_FIFO_KHR
		};
		chk(vkCreateSwapchainKHR(device, &swapchainCI, nullptr, &swapchain));
		uint32_t imageCount{ 0 };
		chk(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
		images.resize(imageCount);
		chk(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));
		imageViews.resize(imageCount);
		for (auto i = 0; i < imageCount; i++) {
			VkImageViewCreateInfo viewCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, .image = images[i], .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = imageFormat, .subresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1 } };
			chk(vkCreateImageView(device, &viewCI, nullptr, &imageViews[i]));
		}
	}

	SwapChain::~SwapChain()
	{

	}
}
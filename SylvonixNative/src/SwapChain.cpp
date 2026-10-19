#include <SwapChain.hpp>
#include <DebugChecks.hpp>
#include <Device.hpp>
#include <Window.hpp>
#include <Image.hpp>

namespace SylvonixNative
{
	SwapChain::SwapChain(SwapChainVulkanObjects objects)
	{
		this->device = objects.device;
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
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
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

		// Depth
		depthImage = std::make_unique<Image>(
			objects.allocator,
			objects.device->getDevice(),
			ImageDescriptor{
				.width = swapchainExtent.width,
				.height = swapchainExtent.height,
				.format = VK_FORMAT_D32_SFLOAT,
				.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				.aspect = VK_IMAGE_ASPECT_DEPTH_BIT,
				.dedicatedMemory = true
			}
		);

		// HDR
		hdrImage = std::make_unique<Image>(
			objects.allocator,
			objects.device->getDevice(),
			ImageDescriptor{
				.width = swapchainExtent.width,
				.height = swapchainExtent.height,
				.format = VK_FORMAT_R16G16B16A16_SFLOAT,
				.usage =
					VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
					VK_IMAGE_USAGE_STORAGE_BIT |
					VK_IMAGE_USAGE_SAMPLED_BIT,
				.aspect = VK_IMAGE_ASPECT_COLOR_BIT,
				.dedicatedMemory = true
			}
		);

		// SDR
		sdrImage = std::make_unique<Image>(
			objects.allocator,
			objects.device->getDevice(),
			ImageDescriptor{
				.width = swapchainExtent.width,
				.height = swapchainExtent.height,
				.format = VK_FORMAT_B8G8R8A8_UNORM,
				.usage =
					VK_IMAGE_USAGE_STORAGE_BIT |
					VK_IMAGE_USAGE_SAMPLED_BIT |
					VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
					VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				.aspect = VK_IMAGE_ASPECT_COLOR_BIT,
				.dedicatedMemory = true
			}
		);
	}

	SwapChain::~SwapChain()
	{
		std::cout << "Destroying HDR\n";
		hdrImage.reset();

		std::cout << "Destroying Depth\n";
		depthImage.reset();

		std::cout << "Destroying Views\n";
		for(auto& imageView : imageViews)
		{
			vkDestroyImageView(
				device->getDevice(),
				imageView,
				nullptr
			);
		}

		std::cout << "Destroying View\n";
		imageViews.clear();

		if(swapchain)
		{
			std::cout << "Destroying Swapchain\n";
			vkDestroySwapchainKHR(
				device->getDevice(),
				swapchain,
				nullptr
			);

			swapchain = VK_NULL_HANDLE;
		}
	}
}
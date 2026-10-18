#include <Image.hpp>
#include <DebugChecks.hpp>

namespace SylvonixNative
{
    Image::Image(VmaAllocator allocator, VkDevice device, const ImageDescriptor& desc)
    {
        this->allocator = allocator;
        this->device = device;
        format = desc.format;

        VkImageCreateInfo imageCI{
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = format,
            .extent{
                desc.width,
                desc.height,
                1
            },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = desc.samples,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = desc.usage,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VmaAllocationCreateInfo allocCI{
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        if (desc.dedicatedMemory)
        {
            allocCI.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        }

        chk(vmaCreateImage(allocator, &imageCI, &allocCI, &image, &allocation, nullptr));

        VkImageViewCreateInfo viewCI{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = desc.format,
            .subresourceRange{
                .aspectMask = desc.aspect,
                .levelCount = 1,
                .layerCount = 1
            }
        };

        chk(vkCreateImageView(device, &viewCI, nullptr, &view));
    }

    Image::~Image()
    {
        Destroy();
    }

    void Image::Destroy()
    {
        if(view)
        {
            vkDestroyImageView(
                device,
                view,
                nullptr
            );

            view = VK_NULL_HANDLE;
        }

        if(image)
        {
            vmaDestroyImage(
                allocator,
                image,
                allocation
            );

            image = VK_NULL_HANDLE;
        }
    }
}
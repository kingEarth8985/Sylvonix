#pragma once
#include <VulkanCommon.hpp>
#include <Descriptors.hpp>

namespace SylvonixNative
{
    class Image
    {
    public:
        Image(VmaAllocator allocator, VkDevice device, const ImageDescriptor& desc);

        ~Image();

        VkImage getImage() { return image; }
        VkImageView getView() { return view; }
        VkFormat getFormat() { return format; }

    private:

        void Destroy();
        
        VkImage image;
        VkImageView view;

        VkFormat format;

        VmaAllocation allocation;

        VmaAllocator allocator;
        VkDevice device;
    };
}
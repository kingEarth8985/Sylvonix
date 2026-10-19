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

        void Transition( VkCommandBuffer cb, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess);

        void PrepareForRender(VkCommandBuffer cb);
        void PrepareForSampling(VkCommandBuffer cb);
        void PrepareForStorageRead(VkCommandBuffer cb);
        void PrepareForStorageWrite(VkCommandBuffer cb);
        void PrepareForCopySrc(VkCommandBuffer cb);
        void PrepareForCopyDst(VkCommandBuffer cb);

        VkImage GetImage() { return image; }
        VkImageView GetView() { return view; }

        VkFormat GetFormat() { return format; }
        VkExtent2D GetExtent() { return extent; }

    private:

        void Destroy();
        
        VkImage image{};
        VkImageView view{};

        VkFormat format{};
        VkExtent2D extent{};

        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageAspectFlags aspect{};

        VmaAllocation allocation;

        VmaAllocator allocator;
        VkDevice device;
    };
}
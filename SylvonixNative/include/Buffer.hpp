#pragma once

#include <VulkanCommon.hpp>
#include <Descriptors.hpp>

namespace SylvonixNative
{
    class Buffer
    {
    public:
        Buffer(VmaAllocator allocator, const BufferDescriptor& desc);

        ~Buffer();

        void* GetMappedData() const { return allocationInfo.pMappedData; }

        VkBuffer GetBuffer() const { return buffer; }

        VkDeviceSize GetSize() const
        {
            return size;
        }

        void Upload(
            const void* data,
            size_t bytes)
        {
            memcpy(
                allocationInfo.pMappedData,
                data,
                bytes
            );
        }

        template<typename T>
        void Set(const T& data)
        {
            Upload(&data, sizeof(T));
        }

    private:
        VmaAllocator allocator{};

        VkDeviceSize size{};

        VmaAllocation allocation{};

        VmaAllocationInfo allocationInfo{};

        VkBuffer buffer{};
    };
}
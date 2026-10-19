#include <Buffer.hpp>
#include <DebugChecks.hpp>

namespace SylvonixNative
{
    Buffer::Buffer(
        VmaAllocator allocator,
        const BufferDescriptor& desc)
    {
        this->allocator = allocator;
        this->size = desc.size;

        VkBufferCreateInfo bufferCI{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = desc.size,
            .usage = desc.usage
        };

        VmaAllocationCreateInfo allocCI{
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        if(desc.mapped)
        {
            allocCI.flags |=
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

            allocCI.flags |=
                VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }

        if(desc.dedicatedMemory)
        {
            allocCI.flags |=
                VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        }

        chk(vmaCreateBuffer(
            allocator,
            &bufferCI,
            &allocCI,
            &buffer,
            &allocation,
            &allocationInfo
        ));
    }

    Buffer::~Buffer()
    {
        if(buffer)
        {
            vmaDestroyBuffer(
                allocator,
                buffer,
                allocation
            );

            buffer = VK_NULL_HANDLE;
        }
    }
}
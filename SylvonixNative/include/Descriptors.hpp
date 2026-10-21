#pragma once
#include <string>
#include <Type.hpp>

namespace SylvonixNative
{
	class Shader;

	struct WindowDescriptor
	{
		std::string title;
		int width;
		int height;
	};

	struct ApplicationDescriptor
	{
		std::string name;
		SoftwareVersion version;

		WindowDescriptor windowDescriptor;
	};

    struct ImageDescriptor
    {
        uint32_t width;
        uint32_t height;

        VkFormat format;

        VkImageUsageFlags usage;

        VkImageAspectFlags aspect;

        VkImageType type = VK_IMAGE_TYPE_2D;

        VkSampleCountFlagBits samples =
            VK_SAMPLE_COUNT_1_BIT;

		bool dedicatedMemory = false;
    };

	struct BufferDescriptor
    {
        VkDeviceSize size{};

        VkBufferUsageFlags usage{};

        bool mapped{ false };

        bool dedicatedMemory{ false };
    };

	struct ComputePipelineDescriptor
    {
        Shader* shader{};
    };
}
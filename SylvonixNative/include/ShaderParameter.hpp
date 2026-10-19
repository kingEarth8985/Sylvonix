#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace SylvonixNative
{
    enum class ShaderParameterType
    {
        Unknown,

        StorageImage,
        SampledImage,

        StorageBuffer,
        UniformBuffer,

        Sampler
    };

    struct ShaderParameter
    {
        std::string name;

        uint32_t binding{};
        uint32_t set{};

        uint32_t size{};

        ShaderParameterType type{};
    };
}
#pragma once

#include <unordered_map>

#include <Shader.hpp>
#include <Image.hpp>
#include <Buffer.hpp>

#include <Descriptors.hpp>

namespace SylvonixNative
{
    class ComputePipeline
    {
    public:

        ComputePipeline(
            VkDevice device,
            const ComputePipelineDescriptor&
                descriptor
        );

        ~ComputePipeline();

        void Set(
            const std::string& name,
            Image* image
        );

        void Set(
            const std::string& name,
            Buffer* buffer
        );

        void Dispatch(
            VkCommandBuffer cb,
            uint32_t x,
            uint32_t y,
            uint32_t z
        );

    private:

        void BuildDescriptorSetLayout();

        void AllocateDescriptors();

        void UpdateDescriptors();

    private:

        struct Resource
        {
            Image* image{};
            Buffer* buffer{};
        };

        VkDevice device{};

        Shader* shader{};

        VkPipeline pipeline{};
        VkPipelineLayout pipelineLayout{};

        VkDescriptorSetLayout
            descriptorSetLayout{};

        VkDescriptorPool
            descriptorPool{};

        VkDescriptorSet
            descriptorSet{};

        std::unordered_map<
            std::string,
            Resource
        > resources;
    };
}
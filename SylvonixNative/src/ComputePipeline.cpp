#include <ComputePipeline.hpp>

#include <DebugChecks.hpp>

namespace SylvonixNative
{
    ComputePipeline::ComputePipeline(
        VkDevice device,
        const ComputePipelineDescriptor& descriptor)
    {
        this->device = device;
        this->shader = descriptor.shader;

        BuildDescriptorSetLayout();

        AllocateDescriptors();

        VkPipelineLayoutCreateInfo layoutCI{
            .sType =
                VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayout
        };

        chk(vkCreatePipelineLayout(
            device,
            &layoutCI,
            nullptr,
            &pipelineLayout
        ));

        VkPipelineShaderStageCreateInfo stageCI{
            .sType =
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage =
                VK_SHADER_STAGE_COMPUTE_BIT,
            .module =
                shader->GetModule(),
            .pName = "main"
        };

        VkComputePipelineCreateInfo pipelineCI{
            .sType =
                VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage = stageCI,
            .layout = pipelineLayout
        };

        chk(vkCreateComputePipelines(
            device,
            VK_NULL_HANDLE,
            1,
            &pipelineCI,
            nullptr,
            &pipeline
        ));
    }

    ComputePipeline::~ComputePipeline()
    {
        if (pipeline)
        {
            vkDestroyPipeline(
                device,
                pipeline,
                nullptr
            );
        }

        if (pipelineLayout)
        {
            vkDestroyPipelineLayout(
                device,
                pipelineLayout,
                nullptr
            );
        }

        if (descriptorPool)
        {
            vkDestroyDescriptorPool(
                device,
                descriptorPool,
                nullptr
            );
        }

        if (descriptorSetLayout)
        {
            vkDestroyDescriptorSetLayout(
                device,
                descriptorSetLayout,
                nullptr
            );
        }
    }

    void ComputePipeline::Set(
        const std::string& name,
        Image* image)
    {
        resources[name].image = image;
    }

    void ComputePipeline::Set(
        const std::string& name,
        Buffer* buffer)
    {
        resources[name].buffer = buffer;
    }

    void ComputePipeline::Dispatch(
        VkCommandBuffer cb,
        uint32_t x,
        uint32_t y,
        uint32_t z)
    {
        UpdateDescriptors();

        vkCmdBindPipeline(
            cb,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            pipeline
        );

        vkCmdBindDescriptorSets(
            cb,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            pipelineLayout,
            0,
            1,
            &descriptorSet,
            0,
            nullptr
        );

        vkCmdDispatch(
            cb,
            x,
            y,
            z
        );
    }

    void ComputePipeline::BuildDescriptorSetLayout()
    {
        std::vector<VkDescriptorSetLayoutBinding>
            bindings;

        for (const auto& parameter :
            shader->GetParameters())
        {
            VkDescriptorSetLayoutBinding binding{};

            binding.binding =
                parameter.binding;

            binding.descriptorCount = 1;

            binding.stageFlags =
                VK_SHADER_STAGE_COMPUTE_BIT;

            switch(parameter.type)
            {
                case ShaderParameterType::StorageImage:
                {
                    binding.descriptorType =
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                    break;
                }

                case ShaderParameterType::SampledImage:
                {
                    binding.descriptorType =
                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    break;
                }

                case ShaderParameterType::StorageBuffer:
                {
                    binding.descriptorType =
                        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                    break;
                }

                case ShaderParameterType::UniformBuffer:
                {
                    binding.descriptorType =
                        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    break;
                }

                default:
                    continue;
            }

            bindings.push_back(
                binding
            );
        }

        VkDescriptorSetLayoutCreateInfo ci{
            .sType =
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount =
                static_cast<uint32_t>(
                    bindings.size()
                ),
            .pBindings =
                bindings.data()
        };

        chk(vkCreateDescriptorSetLayout(
            device,
            &ci,
            nullptr,
            &descriptorSetLayout
        ));
    }

    void ComputePipeline::AllocateDescriptors()
    {
        std::vector<VkDescriptorPoolSize>
            poolSizes;

        for(const auto& parameter :
            shader->GetParameters())
        {
            VkDescriptorPoolSize size{};

            size.descriptorCount = 1;

            switch(parameter.type)
            {
                case ShaderParameterType::StorageImage:
                    size.type =
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                    break;

                case ShaderParameterType::SampledImage:
                    size.type =
                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    break;

                case ShaderParameterType::StorageBuffer:
                    size.type =
                        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                    break;

                case ShaderParameterType::UniformBuffer:
                    size.type =
                        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    break;

                default:
                    continue;
            }

            poolSizes.push_back(
                size
            );
        }

        VkDescriptorPoolCreateInfo poolCI{
            .sType =
                VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets = 1,
            .poolSizeCount =
                static_cast<uint32_t>(
                    poolSizes.size()
                ),
            .pPoolSizes =
                poolSizes.data()
        };

        chk(vkCreateDescriptorPool(
            device,
            &poolCI,
            nullptr,
            &descriptorPool
        ));

        VkDescriptorSetAllocateInfo allocCI{
            .sType =
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool =
                descriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts =
                &descriptorSetLayout
        };

        chk(vkAllocateDescriptorSets(
            device,
            &allocCI,
            &descriptorSet
        ));
    }

    void ComputePipeline::UpdateDescriptors()
    {
        std::vector<VkWriteDescriptorSet>
            writes;

        std::vector<VkDescriptorImageInfo>
            imageInfos;

        std::vector<VkDescriptorBufferInfo>
            bufferInfos;

        for (const auto& parameter :
            shader->GetParameters())
        {
            auto found =
                resources.find(
                    parameter.name
                );

            if(found ==
                resources.end())
            {
                continue;
            }

            const auto& resource =
                found->second;

            if(resource.image)
            {
                imageInfos.push_back({
                    .sampler =
                        VK_NULL_HANDLE,
                    .imageView =
                        resource.image->GetView(),
                    .imageLayout =
                        VK_IMAGE_LAYOUT_GENERAL
                });

                writes.push_back({
                    .sType =
                        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet =
                        descriptorSet,
                    .dstBinding =
                        parameter.binding,
                    .descriptorCount = 1,
                    .descriptorType =
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                    .pImageInfo =
                        &imageInfos.back()
                });
            }

            if(resource.buffer)
            {
                bufferInfos.push_back({
                    .buffer =
                        resource.buffer->GetBuffer(),
                    .offset = 0,
                    .range =
                        resource.buffer->GetSize()
                });

                VkDescriptorType type =
                    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

                if(parameter.type ==
                    ShaderParameterType::
                        UniformBuffer)
                {
                    type =
                        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                }

                writes.push_back({
                    .sType =
                        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .dstSet =
                        descriptorSet,
                    .dstBinding =
                        parameter.binding,
                    .descriptorCount = 1,
                    .descriptorType =
                        type,
                    .pBufferInfo =
                        &bufferInfos.back()
                });
            }
        }

        if(!writes.empty())
        {
            vkUpdateDescriptorSets(
                device,
                static_cast<uint32_t>(
                    writes.size()
                ),
                writes.data(),
                0,
                nullptr
            );
        }
    }
}
#include <Shader.hpp>
#include <DebugChecks.hpp>

namespace SylvonixNative
{
    Shader::Shader(
        VkDevice device,
        slang::IGlobalSession* globalSession,
        const std::string& path,
        ShaderType type)
    {
        this->device = device;

        slang::TargetDesc targetDesc{};
        targetDesc.format = SLANG_SPIRV;
        targetDesc.profile =
            globalSession->findProfile("spirv_1_4");

        slang::CompilerOptionEntry compilerOption{
            slang::CompilerOptionName::EmitSpirvDirectly,
            {
                slang::CompilerOptionValueKind::Int,
                1
            }
        };

        slang::SessionDesc sessionDesc{};

        sessionDesc.targets = &targetDesc;
        sessionDesc.targetCount = 1;

        sessionDesc.defaultMatrixLayoutMode =
            SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;

        sessionDesc.compilerOptionEntries =
            &compilerOption;

        sessionDesc.compilerOptionEntryCount = 1;

        Slang::ComPtr<slang::ISession> session;

        SlangResult createResult =
            globalSession->createSession(
                sessionDesc,
                session.writeRef());

        if (SLANG_FAILED(createResult))
        {
            throw std::runtime_error(
                "Failed to create Slang session.");
        }

        auto* rawModule =
            session->loadModuleFromSource(
                "Shader",
                path.c_str(),
                nullptr,
                nullptr);

        Slang::ComPtr<slang::IModule> slangModule;
        slangModule = rawModule;

        if (!slangModule)
        {
            throw std::runtime_error(
                "Failed to load shader: " + path);
        }

        Slang::ComPtr<slang::IComponentType> program;
        program = slangModule;

        auto* layout = program->getLayout();

        Reflect(layout);

        Slang::ComPtr<ISlangBlob> spirv;

        SlangResult codeResult =
            slangModule->getTargetCode(
                0,
                spirv.writeRef());

        if (SLANG_FAILED(codeResult))
        {
            throw std::runtime_error(
                "Failed to generate SPIR-V.");
        }

        VkShaderModuleCreateInfo shaderModuleCI{};
        shaderModuleCI.sType =
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

        shaderModuleCI.codeSize =
            spirv->getBufferSize();

        shaderModuleCI.pCode =
            reinterpret_cast<
                const uint32_t*>(
                    spirv->getBufferPointer());

        chk(vkCreateShaderModule(
            device,
            &shaderModuleCI,
            nullptr,
            &module));
    }

    Shader::~Shader()
    {
        std::cout << "exiting shader \n";
        if (module)
        {
            vkDestroyShaderModule(
                device,
                module,
                nullptr);

            module = VK_NULL_HANDLE;
        }
        std::cout << "shader destroyed\n";
    }

    const ShaderParameter*
    Shader::FindParameter(
        const std::string& name) const
    {
        for (const auto& parameter : parameters)
        {
            if (parameter.name == name)
            {
                return &parameter;
            }
        }

        return nullptr;
    }

    void Shader::Reflect(
    slang::ProgramLayout* layout)
    {
        parameters.clear();

        if(!layout)
        {
            return;
        }

        SlangInt parameterCount =
            layout->getParameterCount();

        for(
            SlangInt i = 0;
            i < parameterCount;
            i++)
        {
            auto* parameter =
                layout->getParameterByIndex(i);

            auto* typeLayout =
                parameter->getTypeLayout();

            auto* type =
                typeLayout->getType();

            ShaderParameter p{};

            p.name = parameter->getName();

            if(parameter->getCategoryCount() > 0)
            {
                auto category =
                    parameter->getCategoryByIndex(0);

                p.binding =
                    static_cast<uint32_t>(
                        parameter->getOffset(category)
                    );

                p.set =
                    static_cast<uint32_t>(
                        parameter->getBindingSpace(category)
                    );
            }

            switch(type->getKind())
            {
                case slang::TypeReflection::Kind::Resource:
                {
                    if(type->getResourceAccess()
                        ==
                        SLANG_RESOURCE_ACCESS_READ_WRITE)
                    {
                        p.type =
                            ShaderParameterType::
                                StorageImage;
                    }
                    else
                    {
                        p.type =
                            ShaderParameterType::
                                SampledImage;
                    }

                    break;
                }

                case slang::TypeReflection::Kind::ConstantBuffer:
                {
                    p.type =
                        ShaderParameterType::
                            UniformBuffer;

                    break;
                }

                default:
                {
                    p.type =
                        ShaderParameterType::
                            Unknown;

                    break;
                }
            }

            parameters.push_back(p);
        }
    }
}
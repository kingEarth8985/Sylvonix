#pragma once
#include <Buffer.hpp>

namespace SylvonixNative
{
    class GPUMesh
    {
    public:

        Buffer vertexBuffer;
        Buffer indexBuffer;

        Buffer primitiveBuffer;

        std::vector<uint32_t> primitivePBRMaterialIds;
    };

    class GPUMeshInstance
    {
    public:

        
        
    private:
        

        
    };
}
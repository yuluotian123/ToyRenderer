#pragma once
#include "OpenGLInterface\Material.h"
//子类目前的想法：特有的参数 方法(需要多继承吗)
class PBRMaterial:
    public Material
{
public:
    PBRMaterial() {};

    virtual void UpdateUniform() override;

    virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) override;

private:
    std::vector<Texture> textureList;
};
REGISTER(PBRMaterial)
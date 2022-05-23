#pragma once
#include "OpenGLInterface\Material.h"
class SSSMaterial :
    public Material
{
public:
    SSSMaterial() {};

    virtual void UpdateUniform() override;
    virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) override;
    virtual void ShowMaterialProperties(int id) override;

private:
    std::vector<Texture> textureList;
};
REGISTER(SSSMaterial)

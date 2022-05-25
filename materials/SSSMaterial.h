#pragma once
#include "OpenGLInterface\Material.h"
//materialÄÚ²¿·Öpass£¿
class SSSMaterial :
    public Material
{
public:
    SSSMaterial() {};

    virtual void UpdateUniform() override;
    virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) override;
    virtual void ShowMaterialProperties(int id) override;

private:
    std::unordered_map<std::string, Texture> textureMap;

    glm::vec3 color;
};
REGISTER(SSSMaterial)

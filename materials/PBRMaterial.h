#pragma once
#include "OpenGLInterface\Material.h"
//Ȩ��֮��1��������Ҫ�õ�string��ǰ����
namespace pbrUniformNameList {
    const std::string Metal = "Metallic";
    const std::string Rough = "Roughness";
    const std::string useMetalMap = "useMetalMap";
    const std::string useRoughMap = "useRoughMap";
    const std::string useNormalMap = "useNormalMap";
    const std::string useUnKnownMap = "useUnKnownMap";
};


class PBRMaterial:
    public Material
{
public:
    PBRMaterial() {};

    virtual void UpdateUniform() override;
    virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) override;
    virtual void ShowMaterialProperties(int id) override;

private:
    std::vector<Texture> textureList;
    float Metallic = 0.5f;
    float Roughness = 0.5f;
    bool useMetalmap = false;
    bool useRoughmap = false;
    bool useNormalmap = false;
    bool useUnKnownmap = true;//ʵ������metalrough map
};
REGISTER(PBRMaterial)
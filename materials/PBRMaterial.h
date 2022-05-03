#pragma once
#include "OpenGLInterface\Material.h"
//权宜之计1：把所有要用的string提前定义
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
    bool useUnKnownmap = true;//实际上是metalrough map
};
REGISTER(PBRMaterial)
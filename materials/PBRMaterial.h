#pragma once
#include "OpenGLInterface\Material.h"
//权宜之计1：把所有要用的string提前定义
namespace pbrUniformNameList {
    const std::string IBL = "useIBL";

    const std::string Metal = "Metallic";
    const std::string Rough = "Roughness";
    const std::string useMetalMap = "useMetalMap";
    const std::string useRoughMap = "useRoughMap";
    const std::string useNormalMap = "useNormalMap";
    const std::string useMetalRoughMap = "useMetalRoughMap";

    const std::string DiffuseMap = "DiffuseMap0";
    const std::string MetalRoughMap = "MetalRoughMap0";
    const std::string NormalMap = "NormalMap0";
    const std::string MetalMap = "MetalMap0";
    const std::string RoughMap = "RoughMap0";
    const std::string EmissiveMap = "EmissiveMap0";
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
    bool useMetalRoughmap = true;//实际上是metalrough map
    bool IBL = true;
};
REGISTER(PBRMaterial)
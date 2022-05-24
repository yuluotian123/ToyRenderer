#pragma once
#include "OpenGLInterface\Material.h"
//权宜之计1：把所有要用的string提前定义
//预留贴图位置为9个,再多就会抢占其他贴图位
namespace pbrUniformNameList {
    const std::string Metal = "Metallic";
    const std::string Rough = "Roughness";
    const std::string useMetalMap = "useMetalMap";
    const std::string useRoughMap = "useRoughMap";
    const std::string useNormalMap = "useNormalMap";
    const std::string useMetalRoughMap = "useMetalRoughMap";
    const std::string IBL = "useIBL";


    const std::vector<std::string> TextureNameList{
     "DiffuseMap0",
     "MetalRoughMap0",
     "NormalMap0",
     "MetalMap0",
     "RoughMap0",
     "EmissiveMap0"
    };
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
    std::unordered_map<std::string, Texture> textureMap;

    float Metallic = 0.5f;
    float Roughness = 0.5f;

    bool useMetalmap = false;
    bool useRoughmap = false;
    bool useNormalmap = false;
    bool useMetalRoughmap =true;//实际上是unknown map
    bool IBL = true;
};
REGISTER(PBRMaterial)
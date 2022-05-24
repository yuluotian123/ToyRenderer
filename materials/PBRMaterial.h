#pragma once
#include "OpenGLInterface\Material.h"
//Ȩ��֮��1��������Ҫ�õ�string��ǰ����
//Ԥ����ͼλ��Ϊ9��,�ٶ�ͻ���ռ������ͼλ
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
    bool useMetalRoughmap =true;//ʵ������unknown map
    bool IBL = true;
};
REGISTER(PBRMaterial)
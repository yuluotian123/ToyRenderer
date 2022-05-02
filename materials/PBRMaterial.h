#pragma once
#include "OpenGLInterface\Material.h"
//����Ŀǰ���뷨�����еĲ��� ����(��Ҫ��̳���)
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
#pragma once
#include "PBRMaterial.h"

//material�ڲ���pass��
//Ԥ����� Ƥ���α��淴�����
class SSSMaterial :
    public PBRMaterial
{
public:
    SSSMaterial() {};

    virtual void UpdateUniform() override;
    virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) override;
    virtual void ShowMaterialProperties(int id) override;

private:
    static Texture DSLut;//TO DO����json�м���Texture����֪���Ƿ����
    static Texture SSLut;
};

#pragma once
#include "PBRMaterial.h"

//material内部分pass？
//预计算的 皮肤次表面反射材质
class SSSMaterial :
    public PBRMaterial
{
public:
    SSSMaterial() {};

    virtual void UpdateUniform() override;
    virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) override;
    virtual void ShowMaterialProperties(int id) override;

private:
    static Texture DSLut;//TO DO：在json中加入Texture（不知道是否合理）
    static Texture SSLut;
};

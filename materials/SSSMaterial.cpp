#include "SSSMaterial.h"

void SSSMaterial::UpdateUniform()
{




}

void SSSMaterial::RegisterMeshData(const std::vector<Texture>& MeshTexture)
{



}

void SSSMaterial::ShowMaterialProperties(int id)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	for (auto& tPair : textureMap) {
		TREETEXTURE(tPair.second.typeName, tPair.second.ID, flags);
	}

	TREEINPUT(color, Float3, -1 * (id + 1),flags);
}

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "PBRMaterial.h"
#include "OpenGLInterface\Shader.h"

void PBRMaterial::UpdateUniform()//可以不逐实例？但是每个material实例的设置也可能不同
{
	//shader的use被放在了model里
	getShader()->setFloat(pbrUniformNameList::Metal, Metallic);
	getShader()->setFloat(pbrUniformNameList::Rough, Roughness);

	getShader()->setBool(pbrUniformNameList::useMetalMap, useMetalmap);
	getShader()->setBool(pbrUniformNameList::useNormalMap, useNormalmap);
	getShader()->setBool(pbrUniformNameList::useRoughMap, useRoughmap);
	getShader()->setBool(pbrUniformNameList::useMetalRoughMap, useMetalRoughmap);
	getShader()->setBool(pbrUniformNameList::IBL,IBL);

	int i = 0;
	for (auto& tPair:textureMap) {
		glActiveTexture(GL_TEXTURE0+i);
		getShader()->setInt(tPair.second.typeName,i);
		glBindTexture(GL_TEXTURE_2D, tPair.second.ID);
		i++;
	}
}

void PBRMaterial::RegisterMeshData(const std::vector<Texture>& MeshTexture)
{

	for (unsigned i = 0; i < pbrUniformNameList::TextureNameList.size(); i++) {
		textureMap[pbrUniformNameList::TextureNameList[i]] = getDefaultTexture();
		textureMap[pbrUniformNameList::TextureNameList[i]].typeName = pbrUniformNameList::TextureNameList[i];
	}

	for (auto& texture : MeshTexture) {
		for (auto& s : pbrUniformNameList::TextureNameList) {
			if (texture.typeName == s) {
				textureMap[s] = texture;
				break;
			}
		}
	}
}

void PBRMaterial::ShowMaterialProperties(int id)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	int i = 0;
	for (auto& tPair:textureMap) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx(tPair.second.typeName.c_str(), flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::Text(tPair.second.path.c_str());
		ImGui::NextColumn();

		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::Text("%d", tPair.second.ID);
		ImGui::NextColumn();
	}

	TREEBOOL(useMetalmap,-1 * (id + 1))  
	TREEBOOL(useRoughmap, -2 * (id + 1))
	TREEBOOL(useNormalmap, -3 * (id + 1))
	TREEBOOL(useMetalRoughmap, -4 * (id + 1))
	TREEBOOL(IBL, -5 * (id + 1))
    
	TREESLIDE(Metallic,Float, -6 * (id + 1),0,1)
	TREESLIDE(Roughness, Float, -7 * (id + 1), 0, 1)
}

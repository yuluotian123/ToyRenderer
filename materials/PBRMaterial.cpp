#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "PBRMaterial.h"
#include "OpenGLInterface\Shader.h"

void PBRMaterial::UpdateUniform()//也许 先把数据用glGetUniformLocation(ID, name.c_str())注册好比较好
{
	getShader()->Use();
	

	getShader()->setFloat(pbrUniformNameList::Metal, Metallic);
	getShader()->setFloat(pbrUniformNameList::Rough, Roughness);

	getShader()->setBool(pbrUniformNameList::useMetalMap, useMetalmap);
	getShader()->setBool(pbrUniformNameList::useNormalMap, useNormalmap);
	getShader()->setBool(pbrUniformNameList::useRoughMap, useRoughmap);
	getShader()->setBool(pbrUniformNameList::useUnKnownMap, useUnKnownmap);

	for (unsigned int i = 0; i < textureList.size(); i++) {
		glActiveTexture(GL_TEXTURE0+i);
		getShader()->setInt(textureList[i].uniformName,i);
		glBindTexture(GL_TEXTURE_2D, textureList[i].ID);
	}
}

void PBRMaterial::RegisterMeshData(const std::vector<Texture>& MeshTexture)
{
	textureList = MeshTexture;
}

void PBRMaterial::ShowMaterialProperties(int id)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	for (unsigned int i = 0; i < textureList.size(); i++) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx(textureList[i].uniformName.c_str(), flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::Text(textureList[i].path.c_str());
		ImGui::NextColumn();

		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::Text("%d", textureList[i].ID);
		ImGui::NextColumn();
	}

	TREEBOOL(useMetalmap,-1 * (id + 1))  
	TREEBOOL(useRoughmap, -2 * (id + 1))
	TREEBOOL(useNormalmap, -3 * (id + 1))
	TREEBOOL(useUnKnownmap, -4 * (id + 1))
    
	TREESLIDE(Metallic,Float, -5 * (id + 1),0,1)
	TREESLIDE(Roughness, Float, -6 * (id + 1), 0, 1)
}

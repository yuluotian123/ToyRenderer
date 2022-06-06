#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "PBRMaterial.h"
#include "OpenGLInterface\Shader.h"

REGISTER(PBRMaterial)

void PBRMaterial::UpdateUniform()//可以不逐实例？但是每个material实例的设置也可能不同
{
	//shader的use被放在了model里
	getShader()->setFloat(pbrUniformNameList::Metal, Metallic);
	getShader()->setFloat(pbrUniformNameList::Rough, Roughness);
	getShader()->setVec3(pbrUniformNameList::albedo, color);

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
	static bool addTex = false;
	static std::string curChangeTex = "";

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	int i = 1;
	for (auto& tPair:textureMap) {
		TREETEXTURE(tPair.second,tPair.first,i*(id+1),addTex,curChangeTex, flags);
		i++;
	}

	if (addTex) ChangeTex(&addTex, curChangeTex);

	TREEBOOL(useMetalmap,-1 * (id + 1),flags)  
	TREEBOOL(useRoughmap, -2 * (id + 1), flags)
	TREEBOOL(useNormalmap, -3 * (id + 1), flags)
	TREEBOOL(useMetalRoughmap, -4 * (id + 1), flags)
	TREEBOOL(IBL, -5 * (id + 1), flags)
    
	TREEINPUT(color, Float3, -8 * (id + 1), flags)
	TREESLIDE(Metallic,Float, -6 * (id + 1),0,1, flags)
	TREESLIDE(Roughness, Float, -7 * (id + 1), 0, 1, flags)
}

void PBRMaterial::ChangeTex(bool* tex,const std::string& type)
{
	ImGui::Begin("Change Texture");

	static std::string path = "";

	ImGui::Text("Name: %s", type.c_str());

	ImGui::InputText("Path", &path);

	if (ImGui::Button("Change"))
	{
		if (path != "") {
			Texture texture;
			texture.loadTexture(path, false);
			texture.typeName = type;
			if (textureMap[type].path != "./resource/Textures/WhiteTexture/WhiteTexture.png") textureMap[type].Free();
			textureMap[type] = texture;
		}

		*tex = false;
	}

	ImGui::End();
}

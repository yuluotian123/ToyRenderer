#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "SSSMaterial.h"
#include "OpenGLInterface\Shader.h"
REGISTER(SSSMaterial)

Texture SSSMaterial::DSLut;
Texture SSSMaterial::SSLut;
void SSSMaterial::UpdateUniform()
{
	PBRMaterial::UpdateUniform();

	unsigned TexCount = getTextureMap().size() + 1;

	if (DSLut.ID == 0) {
		DSLut.loadDSLUTTexture("./resource/Textures/DiffuseScatteringOnRing.png");
		DSLut.typeName = "dsLUT";
	}
	if (SSLut.ID == 0) {
		SSLut.loadDSLUTTexture("./resource/Textures/SpecularLUT.png");
		SSLut.typeName = "ssLUT";
	}

	glActiveTexture(GL_TEXTURE0+TexCount);
	getShader()->setInt(DSLut.typeName, TexCount);
	glBindTexture(GL_TEXTURE_2D, DSLut.ID);

	glActiveTexture(GL_TEXTURE0 + TexCount+1);
	getShader()->setInt(SSLut.typeName, TexCount+1);
	glBindTexture(GL_TEXTURE_2D, SSLut.ID);

	getShader()->setFloat("scatterScale", Scale);

}

void SSSMaterial::RegisterMeshData(const std::vector<Texture>& MeshTexture)
{
	PBRMaterial::RegisterMeshData(MeshTexture);
}

void SSSMaterial::ShowMaterialProperties(int id)
{
	PBRMaterial::ShowMaterialProperties(id);
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

	TREESLIDE(Scale,Float,1*(id+1),0,10,flags)
}


#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "SSSMaterial.h"
#include "OpenGLInterface\Shader.h"
REGISTER(SSSMaterial)

Texture SSSMaterial::DSLut;
void SSSMaterial::UpdateUniform()
{
	PBRMaterial::UpdateUniform();

	unsigned TexCount = getTextureMap().size() + 1;

	if (DSLut.ID == 0) {
		DSLut.loadDSLUTTexture("./resource/Textures/DiffuseScatteringOnRing.png");
		DSLut.typeName = "dsLUT";
	}

	glActiveTexture(GL_TEXTURE0+TexCount);
	getShader()->setInt(DSLut.typeName, TexCount);
	glBindTexture(GL_TEXTURE_2D, DSLut.ID);

}

void SSSMaterial::RegisterMeshData(const std::vector<Texture>& MeshTexture)
{
	PBRMaterial::RegisterMeshData(MeshTexture);
}

void SSSMaterial::ShowMaterialProperties(int id)
{
	PBRMaterial::ShowMaterialProperties(id);
}


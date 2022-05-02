#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "PBRMaterial.h"
#include "OpenGLInterface\Shader.h"

void PBRMaterial::UpdateUniform()
{
	getShader()->Use();
	
	for (unsigned int i = 0; i < textureList.size(); i++) {
		glActiveTexture(GL_TEXTURE0);
		setInt(textureList[i].uniformName,i);
		glBindTexture(GL_TEXTURE_2D, textureList[i].ID);
	}
}

void PBRMaterial::RegisterMeshData(const std::vector<Texture>& MeshTexture)
{
	textureList = MeshTexture;
}

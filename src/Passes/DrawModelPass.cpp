#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Camera.h"
#include "Manager\SceneManager.h"
#include "Manager\Scene.h"
#include "Passes\DrawModelPass.h"

void DrawModelPass::init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->isMaterial) {
			shaderP.second->Use();
			shaderP.second->setInt("DirShadowMap", RenderManager::getCurGlobalTexNum());
		}
	}
	glActiveTexture(GL_TEXTURE0 + RenderManager::getCurGlobalTexNumAndAdd());
	glBindTexture(GL_TEXTURE_2D_ARRAY, RenderManager::getOrCreateInstance()->getPassDataByName<Texture>("DirDepthTex").ID);

}

void DrawModelPass::update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	RenderTarget::clear();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	context->setupCameraProperties(Rendercamera);
	setClusterProperties(Rendercamera);
	setCSMProperties();

	context->DrawOpaqueRenderList(SceneManager::getOrCreateInstance()->getCurrentScene()->getModels());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void DrawModelPass::setClusterProperties(std::shared_ptr<Camera>& Rendercamera)
{
	unsigned gridSizeX = RenderManager::getOrCreateInstance()->getPassDataByName<unsigned int>("GridSizeX");
	unsigned gridSizeY = RenderManager::getOrCreateInstance()->getPassDataByName<unsigned int>("GridSizeY");
	unsigned gridSizeZ = RenderManager::getOrCreateInstance()->getPassDataByName<unsigned int>("GridSizeZ");

	float zNear = Rendercamera->GetNearPlane();
	float zFar = Rendercamera->GetFarPlane();

	unsigned tileSize = (unsigned int)std::ceilf(SCREEN_WIDTH / (float)gridSizeX);
	float Scale = (float)gridSizeZ / std::log2f(zFar / zNear);
	float Bias = -((float)gridSizeZ * std::log2f(zNear) / std::log2f(zFar / zNear));

	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()) {
		if (shaderP.second->isMaterial) {
			shaderP.second->Use();
			shaderP.second->setInt("GridSizeX", gridSizeX);
			shaderP.second->setInt("GridSizeY", gridSizeY);
			shaderP.second->setInt("GridSizeZ", gridSizeZ);
			shaderP.second->setFloat("Scale", Scale);
			shaderP.second->setFloat("Bias", Bias);
			shaderP.second->setInt("TileSize", tileSize);
		}
	}
}

void DrawModelPass::setCSMProperties()
{
	std::vector<float> FarPlaneList = RenderManager::getOrCreateInstance()->getPassDataByName<std::vector<float>>("DistanceList");

	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()) {
		if (shaderP.second->isMaterial) {
			shaderP.second->Use();
			for (size_t i = 0; i < FarPlaneList.size(); i++)
				shaderP.second->setFloat("PlaneDistances[" + std::to_string(i) + "]", FarPlaneList[i]);

			shaderP.second->setInt("SplitNum", FarPlaneList.size());
		}
	}
}

#include "OpenGLInterface\RenderTarget.h"
#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "Manager\SceneManager.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Camera.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Skybox.h"

bool RenderContext::Init()
{
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();
	SkyBoxshader = MaterialSystem::getOrCreateInstance()->registerShader("skyBoxVs.glsl", "skyBoxFs.glsl");

	if (curScene->getSkybox() && curScene->getSkybox()->IsHDR())
		InitSkyCubeMapFromHDR();

	return true;
}

void RenderContext::DrawOpaqueRenderList()
{
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();

	for (auto& model : curScene->getModels()) {
		setupModelMatrix(model);
		model->Draw();
	}
}

void RenderContext::DrawOpaqueRenderList(Shaderid shaderid)
{
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();

	for (auto& model : curScene->getModels()) {
		setupModelMatrix(model);
		model->Draw(shaderid);
	}
}

void RenderContext::InitSkyCubeMapFromHDR()
{
	std::shared_ptr<Skybox> skybox = curScene->getSkybox();

	EquiRecToCubeshader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl", "EquiRectangularToCubeFs.glsl");
	EquiRecToCubeRT.createRenderTarget(skybox->getResolution(), skybox->getResolution(), RenderTarget::ENUM_TYPE_BASIC);
	EquiRecToCubeRT.use();
	skybox->fillCubeMapWithHDR(EquiRecToCubeshader);
	EquiRecToCubeRT.UseDefault();

}

void RenderContext::DrawSkybox()
{
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();
	std::shared_ptr<Skybox> skybox = curScene->getSkybox();
	std::shared_ptr<Shader> shader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(SkyBoxshader);

	shader->Use();
	shader->setInt("environmentMap",0);
	skybox->draw();
}

void RenderContext::setupCameraProperties(std::shared_ptr<Camera> camera)
{
	for (auto shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useCamera) {
			shaderP.second->Use();
			shaderP.second->setMat4(CameraUniformNameList::view, camera->GetViewMatrix());
			shaderP.second->setMat4(CameraUniformNameList::projection, camera->GetProjectionMatrix());
			shaderP.second->setMat4(CameraUniformNameList::invView, camera->GetInvViewMatrix());
			shaderP.second->setMat4(CameraUniformNameList::invProjection, camera->GetInvProjectionMatrix());
			shaderP.second->setVec3(CameraUniformNameList::CamPos, camera->GetPosition());
			shaderP.second->setInt(CameraUniformNameList::Scrwidth,SCREEN_WIDTH);
			shaderP.second->setInt(CameraUniformNameList::Scrheight, SCREEN_HEIGHT);
		}
	}
}

void RenderContext::setupModelMatrix(std::shared_ptr<Model> model)
{
	for (auto shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useCamera) {
			shaderP.second->Use();
			shaderP.second->setMat4(CameraUniformNameList::model, model->getModelMatrix());
		}
	}
}

void RenderContext::setupLightProperties()
{
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();

	for (auto shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useLight) {
			for (auto& light : curScene->getLights()) {
				if (light->type == "DirectionalLight") {
					shaderP.second->Use();
					shaderP.second->setVec3("LightColor",light->color);
					shaderP.second->setFloat("LightIntensity", light->intensity);
					shaderP.second->setVec3("LightDirection", std::dynamic_pointer_cast<DirectionalLight>(light)->direction);
				}

			}
		}
	}
}

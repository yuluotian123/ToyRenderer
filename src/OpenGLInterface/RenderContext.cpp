#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include "Manager\SceneManager.h"
#include "OpenGLInterface\Camera.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Light.h"
//TO DO：让这些东西只在需要的shader中update

void RenderContext::DrawOpaqueRenderList(const std::vector<std::shared_ptr<Model>>& models)
{
	for (auto model : models)
	{
		setupModelMatrix(model);
		model->Draw();
	}
}

void RenderContext::DrawOpaqueRenderList(const std::vector<std::shared_ptr<Model>>& models, Shaderid shaderid)
{
	for (auto model : models)
	{
		setupModelMatrix(model);
		model->Draw(shaderid);
	}
}

//TO DO：用UBO 
void RenderContext::setupCameraProperties(std::shared_ptr<Camera> camera)
{
	for (auto shader : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()){
		if (shader.second->useCamera) {
			shader.second->Use();
			shader.second->setInt(CameraUniformNameList::Scrwidth, SCREEN_WIDTH);
			shader.second->setInt(CameraUniformNameList::Scrheight, SCREEN_HEIGHT);
			shader.second->setMat4(CameraUniformNameList::view, camera->GetViewMatrix());
			shader.second->setMat4(CameraUniformNameList::projection, camera->GetProjectionMatrix());
			shader.second->setMat4(CameraUniformNameList::invView, camera->GetInvViewMatrix());
			shader.second->setMat4(CameraUniformNameList::invProjection, camera->GetInvProjectionMatrix());
			shader.second->setVec3(CameraUniformNameList::CamPos, camera->GetPosition());
		}
	}
}

//TO DO 只更新model中含有的shader类型(或者把他放到draw方法里去)（没想到什么好办法）
void RenderContext::setupModelMatrix(std::shared_ptr<Model> model)
{
	for (auto shader : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()) {
		shader.second->Use();
		shader.second->setMat4(CameraUniformNameList::model, model->getModelMatrix());
	}
}

void RenderContext::setupLightProperties(const std::vector<std::shared_ptr<Light>>& lights)
{
	for (auto shader : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()) {
		if (shader.second->useLight) {
			shader.second->Use();
			for (auto light : lights) {
				if (light->type == "DirectionalLight") {
					shader.second->setVec3("LightColor", light->color);
					shader.second->setFloat("LightIntensity", light->intensity);
					shader.second->setVec3("LightDirection", std::dynamic_pointer_cast<DirectionalLight>(light)->direction);
				}
			}


		}
	}
}

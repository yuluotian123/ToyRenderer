#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include "Manager\SceneManager.h"
#include "OpenGLInterface\Camera.h"
#include "OpenGLInterface\Model.h"

void RenderContext::setupCameraProperties(std::shared_ptr<Camera> camera)
{
	for (auto shader : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()){
		shader.second->Use();
		shader.second->setMat4("uViewMatrix",camera->GetViewMatrix());
		shader.second->setMat4("uProjectionMatrix", camera->GetProjectionMatrix());
	}
}

//TO DO ֻ����model�к��е�shader����
void RenderContext::setupModelMatrix(std::shared_ptr<Model> model)
{
	for (auto shader : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()) {
		shader.second->Use();
		shader.second->setMat4("uModelMatrix", model->getModelMatrix());
	}
}

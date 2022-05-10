#include "Manager\SceneManager.h"
#include "Manager\RenderManager.h"
#include "OpenGLInterface\Camera.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Light.h"
#include "Passes\ClusterLight.h"
#include "Passes\SkyBoxPass.h"
#include "Passes\DrawModelPass.h"

bool RenderManager::StartUp()
{
	Rendercamera = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainCamera();
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();
	context = std::make_shared<RenderContext>();

	registerPass(std::make_shared<ClusterLight>("ClusterPass",1));
	registerPass(std::make_shared<DrawModelPass>("DrawModelPass", 2));
	registerPass(std::make_shared<SkyBoxPass>("SkyBoxPass", 3));

	for (auto& pass : passes)
		pass->init(context, Rendercamera);

	return true;
}

void RenderManager::ShutDown()
{
	glfwTerminate();
}

void RenderManager::Render(float DeltaTime)
{
	glEnable(GL_DEPTH_TEST);

	RenderTarget::clear();

	setLightProperties();

	for (auto& pass : passes)
		pass->update(context, Rendercamera);
}

void RenderManager::setLightProperties()
{
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useLight) {
			for (auto& light : curScene->getLights()) {
				if (light->type == "DirectionalLight") {
					shaderP.second->Use();
					shaderP.second->setVec3("LightColor", light->color);
					shaderP.second->setFloat("LightIntensity", light->intensity);
					shaderP.second->setVec3("LightDirection", std::dynamic_pointer_cast<DirectionalLight>(light)->direction);
				}
			}
		}
	}
}

void RenderManager::registerPass(const std::shared_ptr<BasePass>& pass)
{
	passes.insert(std::lower_bound(passes.begin(),passes.end(),pass, [](const std::shared_ptr<BasePass>& Pass1, const std::shared_ptr<BasePass>& Pass2) {return Pass1->getOrder() < Pass2->getOrder(); }),pass);
}

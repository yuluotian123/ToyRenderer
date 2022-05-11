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
	RenderTarget::clear();

	setLightProperties();

	for (unsigned i = 0; i < passes.size();i++) {
		if (passes[i]->getOrder() == -1) { i++; continue; }
		switch (passes[i]->getRendertype())
		{
		case RenderPassType::PassType_Normal:
			passes[i]->update(context, Rendercamera);
			break;
		case RenderPassType::PassType_Once:
			passes[i]->update(context, Rendercamera);
			passes[i]->finish();
			break;
		case RenderPassType::PassType_Parallel: {
			int j = i;
			while (j < passes.size())
			{
				if (passes[i]->getOrder() != passes[j]->getOrder())break;
				passes[j++]->update(context, Rendercamera);
			}
			i = j - 1;
			break;
		}
		case RenderPassType::PassType_ParallelOnce: {
			int j = i;
			while (j < passes.size())
			{
				if (passes[i]->getOrder() != passes[j]->getOrder())break;

				passes[j++]->update(context, Rendercamera);
			}
			while (i < j)
			{
				passes[i++]->finish();
			}
			i--;
			break;
		}
		case RenderPassType::PassType_Delay:
			passes[i]->setPassType(RenderPassType::PassType_Normal);
			break;
		}
	}
}

void RenderManager::registerPassData(const std::string& name, std::any data)
{
	if (PassDatas.find(name) != PassDatas.end()) {
		printf("You has already register %s.", name.c_str());
		return;
	}
	PassDatas[name] = data;
}

void RenderManager::updatePassData(const std::string& name, const std::any& data)
{
	PassDatas[name] = data;
}

const std::any& RenderManager::getPassDataByName(const std::string& name) {
	if (PassDatas.find(name) != PassDatas.end()) return PassDatas[name];
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

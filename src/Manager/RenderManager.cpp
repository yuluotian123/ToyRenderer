#include "Manager\SceneManager.h"
#include "Manager\RenderManager.h"
#include "Passes\ClusterLight.h"
#include "Passes\SkyBoxPass.h"
#include "Passes\DrawModelPass.h"
#include "Passes\CSMshadowMap.h"

unsigned RenderManager::curGlobalTexNum = 10;

bool RenderManager::StartUp()
{
	Rendercamera = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainCamera();
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();
	context = std::make_shared<RenderContext>();

	registerPass(std::make_shared<ClusterLight>("ClusterPass",1));
	registerPass(std::make_shared<CSMshadowMap>("ShadowMapPass", 2));
	registerPass(std::make_shared<DrawModelPass>("DrawModelPass", 3));
	registerPass(std::make_shared<SkyBoxPass>("SkyBoxPass", 4));

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
			unsigned int j = i;
			while (j < passes.size())
			{
				if (passes[i]->getOrder() != passes[j]->getOrder())break;
				passes[j++]->update(context, Rendercamera);
			}
			i = j - 1;
			break;
		}
		case RenderPassType::PassType_ParallelOnce: {
			unsigned int j = i;
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
	return NULL;
}

void RenderManager::registerPass(const std::shared_ptr<BasePass>& pass)
{
	passes.insert(std::lower_bound(passes.begin(),passes.end(),pass, [](const std::shared_ptr<BasePass>& Pass1, const std::shared_ptr<BasePass>& Pass2) {return Pass1->getOrder() < Pass2->getOrder(); }),pass);
}

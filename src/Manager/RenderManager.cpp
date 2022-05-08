#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Manager\SceneManager.h"
#include "Manager\RenderManager.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Material.h"
#include "OpenGLInterface\Camera.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Light.h"

bool RenderManager::StartUp()
{
	Rendercamera = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainCamera();
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();

	context = std::make_shared<RenderContext>();
	if (!context->Init()) return false;

	Cluster_AABBpass();

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

	updateLightSSBO();

	context->setupCameraProperties(Rendercamera);

	setLightProperties();
	
	context->DrawOpaqueRenderList();

	context->DrawSkybox();
}

void RenderManager::updateLightSSBO()
{
		glGenBuffers(1, &LightSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, maxLightCount * sizeof(struct GPUpointLight), NULL, GL_DYNAMIC_DRAW);
		GPUpointLight* gpulights = (GPUpointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
		std::vector<std::shared_ptr<Light>> lights = curScene->getLights();
		for (unsigned int i = 0; i < lights.size(); ++i) {
			if (lights[i]->type == "PointLight") {



			}
		}
}

void RenderManager::Cluster_AABBpass()
{
	ClusterShader = MaterialSystem::getOrCreateInstance()->registerComputeShader("ClusterCs.glsl");
	std::shared_ptr<Shader> tShader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ClusterShader);

	tShader->Use();
	tShader->setFloat("NearPlane", Rendercamera->GetNearPlane());
	tShader->setFloat("FarPlane", Rendercamera->GetFarPlane());
	tShader->setMat4("InvProjectionMatrix", Rendercamera->GetInvProjectionMatrix());
	tShader->setInt("ScreenWidth", SCREEN_WIDTH);
	tShader->setInt("ScreenHeight", SCREEN_HEIGHT);
	tShader->Dispatch(gridSizeX, gridSizeY, gridSizeZ);

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

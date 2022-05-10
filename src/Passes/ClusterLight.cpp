#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "Passes\ClusterLight.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Camera.h"
#include "Manager\SceneManager.h"
#include "Manager\Scene.h"

void ClusterLight::init(std::shared_ptr<RenderContext>& context,std::shared_ptr<Camera>& Rendercamera)
{
	ClusterShader = MaterialSystem::getOrCreateInstance()->registerComputeShader("ClusterCs.glsl");
	LightSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, maxLightCount * sizeof(GPUpointLight),NULL, GL_DYNAMIC_DRAW, 0);
}

void ClusterLight::update(std::shared_ptr<RenderContext>& context,std::shared_ptr<Camera>& Rendercamera)
{
	Cluster_AABBPass(Rendercamera);
    
	updateLightSSBO(SceneManager::getOrCreateInstance()->getCurrentScene()->getLights());
}

void ClusterLight::updateLightSSBO(std::vector<std::shared_ptr<Light>>& lights)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightSSBO);
	GPUpointLight* gpulights = (GPUpointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	int i = 0;
	for (auto& light : lights) {
		if (light->type == "PointLight")
		{
			gpulights[i].Color = light->color;
			gpulights[i].Intensity = light->intensity;
			gpulights[i].position = std::dynamic_pointer_cast<PointLight>(light)->position;
			gpulights[i].radius = std::dynamic_pointer_cast<PointLight>(light)->radius;
			i++;
		}
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	PointLightCount = i;
}

void ClusterLight::Cluster_AABBPass(std::shared_ptr<Camera>& Rendercamera)
{
	std::shared_ptr<Shader> AABBshader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ClusterShader);

	AABBshader->Use();
	AABBshader->useCamera = false;
	AABBshader->useIBL = false;
	AABBshader->useLight = false;

	AABBshader->setFloat("NearPlane", Rendercamera->GetNearPlane());
	AABBshader->setFloat("FarPlane", Rendercamera->GetFarPlane());
	AABBshader->setMat4("InvProjectionMatrix", Rendercamera->GetInvProjectionMatrix());
	AABBshader->setInt("ScreenWidth", SCREEN_WIDTH);
	AABBshader->setInt("ScreenHeight", SCREEN_HEIGHT);
	AABBshader->Dispatch(gridSizeX, gridSizeY, gridSizeZ);
}

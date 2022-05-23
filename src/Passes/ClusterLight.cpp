#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "Passes\ClusterLight.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Camera.h"
#include "Manager\SceneManager.h"
#include "Manager\Scene.h"

bool ClusterLight::Update = false;

void ClusterLight::init(std::shared_ptr<RenderContext>& context,std::shared_ptr<Camera>& Rendercamera)
{
	ClusterShader = MaterialSystem::getOrCreateInstance()->registerComputeShader("ClusterCs.glsl");
	LightCullingShader = MaterialSystem::getOrCreateInstance()->registerComputeShader("LightCullingCs.glsl");
	aabbSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, clusterNum * sizeof(struct clusterAABB), NULL, GL_STATIC_COPY, 1);
	LightSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, maxLightCount * sizeof(struct GPUpointLight), NULL, GL_DYNAMIC_DRAW, 2);
	LightListSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, clusterNum * maxLightPerCluster * sizeof(unsigned int), NULL, GL_STATIC_COPY, 3);
	LightGridSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, clusterNum * 2 * sizeof(unsigned int), NULL, GL_STATIC_COPY, 4);
	updateLightSSBO(SceneManager::getOrCreateInstance()->getCurrentScene()->getLights());
	RenderManager::getOrCreateInstance()->registerPassData("GridSizeX", gridSizeX);
	RenderManager::getOrCreateInstance()->registerPassData("GridSizeY", gridSizeY);
	RenderManager::getOrCreateInstance()->registerPassData("GridSizeZ", gridSizeZ);

}

void ClusterLight::update(std::shared_ptr<RenderContext>& context,std::shared_ptr<Camera>& Rendercamera)
{
	setMainLightProperties(SceneManager::getOrCreateInstance()->getCurrentScene()->getLights());

	if(Update) updateLightSSBO(SceneManager::getOrCreateInstance()->getCurrentScene()->getLights());//手动update

	Cluster_AABBPass(Rendercamera);
	LightCullingPass(Rendercamera);//好像有点慢？
}

void ClusterLight::updateLightSSBO(std::vector<std::shared_ptr<Light>>& lights)
{
	int i = 0;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightSSBO);
	GPUpointLight* gpulights = (GPUpointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);//性能消耗很大 如果是静态场景可以不用每帧update，或者手动update
	for (auto& light : lights) {
		if (light->type == "PointLight")
		{
			gpulights[i].position = glm::vec4(std::dynamic_pointer_cast<PointLight>(light)->position,1.0f);
			gpulights[i].color = glm::vec4(light->color, 1.0f);
			gpulights[i].radius = std::dynamic_pointer_cast<PointLight>(light)->radius;
			gpulights[i].intensity = light->intensity;
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
	AABBshader->isMaterial = false;

	unsigned tileSize = (unsigned int)std::ceilf(SCREEN_WIDTH / (float)gridSizeX);
	AABBshader->setInt("TileSize", tileSize);
	AABBshader->setFloat("NearPlane", Rendercamera->GetNearPlane());
	AABBshader->setFloat("FarPlane", Rendercamera->GetFarPlane());
	AABBshader->setMat4("InvProjectionMatrix", Rendercamera->GetInvProjectionMatrix());
	AABBshader->setInt("ScreenWidth", SCREEN_WIDTH);
	AABBshader->setInt("ScreenHeight", SCREEN_HEIGHT);

	AABBshader->Dispatch(gridSizeX, gridSizeY, gridSizeZ);
}

void ClusterLight::LightCullingPass(std::shared_ptr<Camera>& Rendercamera)
{
	std::shared_ptr<Shader> Cullingshader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(LightCullingShader);
	Cullingshader->Use();
	Cullingshader->isMaterial = false;

	Cullingshader->setMat4("ViewMatrix",Rendercamera->GetViewMatrix());
	Cullingshader->setInt("MaxLightPerTile",maxLightPerCluster);
	Cullingshader->setInt("PointLightLength", PointLightCount);

	Cullingshader->Dispatch(gridSizeX, gridSizeY, gridSizeZ);
}

void ClusterLight::setMainLightProperties(std::vector<std::shared_ptr<Light>>& lights)
{
	{
		for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
		{
			if (shaderP.second->isMaterial) {
				for (auto& light : lights) {
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
}

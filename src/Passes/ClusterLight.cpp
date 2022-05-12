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
	LightCullingShader = MaterialSystem::getOrCreateInstance()->registerComputeShader("LightCullingCs.glsl");
	aabbSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, clusterNum * sizeof(clusterAABB), NULL, GL_STATIC_COPY, 1);
	LightSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, maxLightCount * sizeof(GPUpointLight),NULL, GL_DYNAMIC_DRAW, 2);
	LightListSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, clusterNum*maxLightPerCluster * sizeof(unsigned int), NULL, GL_STATIC_COPY, 3);
	LightGridSSBO = context->GenBuffer(GL_SHADER_STORAGE_BUFFER, clusterNum * 2 * sizeof(unsigned int), NULL, GL_STATIC_COPY, 4);

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, aabbSSBO);
	//clusterAABB* p = (clusterAABB*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	//for (int i = 0; i < gridSizeZ; i++) {
	//	for (int i = 0; i < gridSizeY; i++) {
	//		for (int i = 0; i < gridSizeX; i++) {
	//			printf("%f %f %f", (*p).maxPoint.x, (*p).maxPoint.y, (*p).maxPoint.z);
	//			p++;
	//			printf("\n");
	//		}
	//		printf("\n");
	//	}
	//	printf("\n");
	//}


	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ClusterLight::update(std::shared_ptr<RenderContext>& context,std::shared_ptr<Camera>& Rendercamera)
{
	updateLightSSBO(SceneManager::getOrCreateInstance()->getCurrentScene()->getLights());//性能消耗很大 如果是静态场景可以不用update
	Cluster_AABBPass(Rendercamera);
	LightCullingPass(Rendercamera);
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
	Cullingshader->useCamera = false;
	Cullingshader->setMat4("ViewMatrix",Rendercamera->GetViewMatrix());
	Cullingshader->setInt("MaxLightPerTile",maxLightPerCluster);
	Cullingshader->Dispatch(gridSizeX, gridSizeY, gridSizeZ);
}

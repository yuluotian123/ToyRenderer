#pragma once
#include "Base\Singleton.hpp"
#include "OpenGLInterface\RenderContext.h"

class Scene;
class Camera;
class RenderManager:public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
public:
	~RenderManager() {};
	bool StartUp();
	void ShutDown();

	void Render(float DeltaTime);
private:
	void updateLightSSBO();//把所有ssbo都放在一个函数里 比较好看
	void Cluster_AABBpass();
	void setLightProperties();
private:
	RenderManager()=default;
	std::shared_ptr<Camera> Rendercamera;
	std::shared_ptr<Scene> curScene;
private:
	std::shared_ptr<RenderContext> context;

	Shaderid ClusterShader;

	unsigned LightSSBO;
	const unsigned int maxLightCount = 80;
	const unsigned int gridSizeX = 16;
	const unsigned int gridSizeY = 9;
	const unsigned int gridSizeZ = 24;
};


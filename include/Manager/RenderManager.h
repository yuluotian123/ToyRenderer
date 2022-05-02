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
	RenderManager() {};
	std::shared_ptr<Camera> Rendercamera;
	std::shared_ptr<Scene> Scene;

	RenderContext context;
};


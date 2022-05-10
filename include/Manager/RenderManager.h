#pragma once
#include "OpenGLInterface\RenderContext.h"
#include "Base\Singleton.hpp"

class Scene;
class Camera;
class BasePass;
class RenderManager:public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;
public:
	~RenderManager() {};
	bool StartUp();
	void ShutDown();

	void Render(float DeltaTime);
private:
	void setLightProperties();

	void registerPass(const std::shared_ptr<BasePass>& pass);
private:
	RenderManager()=default;
	std::shared_ptr<Camera> Rendercamera;
	std::shared_ptr<Scene> curScene;
private:
	std::shared_ptr<RenderContext> context;
	std::vector <std::shared_ptr<BasePass> > passes;
	//TO DO：全部用pass来管理
};


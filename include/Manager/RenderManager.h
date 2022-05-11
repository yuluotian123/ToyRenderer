#pragma once
#include "OpenGLInterface\RenderContext.h"
#include "Base\Singleton.hpp"
#include <unordered_map>
#include <any>

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

	void registerPassData(const std::string& name, std::any data);
	void updatePassData(const std::string& name, const std::any& data);
	const std::any& getPassDataByName(const std::string& name);
	template <typename DataType>
	DataType getPassDataByName(const std::string& name)
	{
		return std::any_cast<DataType>(getPassDataByName(name));
	}
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

	std::unordered_map<std::string, std::any> PassDatas;//用于在pass间传递shader等一系列成员变量
	//TO DO：全部用pass来管理
};
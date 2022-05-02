#pragma once
#include "Manager\Scene.h"
#include "Base\Singleton.hpp"
#include <string>

//TO DO: SwitchScene
class Scene;
class SceneManager:public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
public:
	~SceneManager() {};

	bool StartUp();
	void ShutDown();
	void Update(float DeltaTime);

	std::shared_ptr<Scene> getCurrentScene() {return curScene;};
private:
	bool loadScene(std::string sceneID);
private:
	std::string curSceneID;
	std::shared_ptr<Scene> curScene = nullptr;
};


#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Base\Singleton.hpp"

class DisplayManager;
class InputManager;
class RenderManager;
class SceneManager;
class Engine:public Singleton<Engine>
{
	friend class Singleton<Engine>;
public:
	~Engine() {};

	bool StartUp();
	void ShutDown();
	void Update();
private:
	void UpdateFrame();
private:
	std::shared_ptr<DisplayManager> Edisplaymanager = nullptr;
	std::shared_ptr<InputManager> Einputmanager = nullptr;
	std::shared_ptr<RenderManager> Erendermanager = nullptr;
	std::shared_ptr<SceneManager> Escenemanager = nullptr;

	GLFWwindow* EWindow = nullptr;

	float deltaTime = 0;
	float lastFrame = 0;

private:
		Engine() {};
};


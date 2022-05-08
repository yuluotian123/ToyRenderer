#pragma once
#include "Manager\DisplayManager.h"
#include "Base\Common.h"

class SceneManager;
class Camera;
static bool firstMouse = true;
static float lastX = SCREEN_WIDTH/ 2.0f;
static float lastY = SCREEN_HEIGHT / 2.0f;
//管理输入
class InputManager:public Singleton<InputManager>
{
	friend class Singleton<InputManager>;
public:
	~InputManager() {};

	bool StartUp();
	void ShutDown();

	void processInput(GLFWwindow* window,float deltatime);
private:
	void registerCallBack(GLFWwindow* window);

	void glfw_keyInput(GLFWwindow* window, float deltatime);
	void glfw_mouseInput(GLFWwindow* window, float deltatime);
private:
	//和imgui兼容性不好
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); };
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
	InputManager() {};
};


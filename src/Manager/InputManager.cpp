#include "Manager\InputManager.h"
#include "Manager\SceneManager.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

static std::shared_ptr<Camera> UsingCamera;

bool InputManager::StartUp()
{
	GLFWwindow* pWindow = DisplayManager::getOrCreateInstance()->getWindow();

	UsingCamera = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainCamera();

	registerCallBack(pWindow);

	return true;
}

void InputManager::registerCallBack(GLFWwindow* window)
{
	//和imgui疑似冲突？需要进一步了解(至少鼠标位置和鼠标点击事件)
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void InputManager::ShutDown()
{
}

void InputManager::processInput(GLFWwindow* window,float deltaTime)
{
	ImGuiIO& io = ImGui::GetIO();
	if(!io.WantCaptureKeyboard)
	    glfw_keyInput(window, deltaTime);
	if (!io.WantCaptureMouse)
		glfw_mouseInput(window, deltaTime);
}

void InputManager::glfw_keyInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		UsingCamera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		UsingCamera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		UsingCamera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		UsingCamera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		UsingCamera->ProcessKeyboard(Camera_Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		UsingCamera->ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

}

void InputManager::glfw_mouseInput(GLFWwindow* window, float deltatime)
{
	if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)== GLFW_PRESS)
	{ 
		float lastX = 0.5f * ImGui::GetIO().MouseDelta.x;
		float lastY = -0.5f * ImGui::GetIO().MouseDelta.y;
		UsingCamera->ProcessMouseMovement(lastX, lastY);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
	{
		/*float lastX = ImGui::GetIO().MouseDelta.x;
		float lastY = ImGui::GetIO().MouseDelta.y;
		UsingCamera->ProcessArcBall(lastX,lastY);*/
	}
}

void InputManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	UsingCamera->ProcessMouseScroll(yoffset);
}

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Manager\SceneManager.h"
#include "Manager\RenderManager.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Material.h"
#include "OpenGLInterface\Camera.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"

bool RenderManager::StartUp()
{
	Rendercamera = SceneManager::getOrCreateInstance()->getCurrentScene()->fecthOrCreateMainCamera();
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();

	return true;
}

void RenderManager::ShutDown()
{
	glfwTerminate();
}

void RenderManager::Render(float DeltaTime)
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	context.setupCameraProperties(Rendercamera);

	context.setupLightProperties(curScene->getLights());
	
	context.DrawOpaqueRenderList(curScene->getModels());
}

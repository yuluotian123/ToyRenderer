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
	Rendercamera = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainCamera();
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();
	context = std::make_shared<RenderContext>();

	if (!context->Init()) return false;

	return true;
}

void RenderManager::ShutDown()
{
	glfwTerminate();
}

void RenderManager::Render(float DeltaTime)
{
	glEnable(GL_DEPTH_TEST);

	RenderTarget::clear();

	context->setupCameraProperties(Rendercamera);

	context->setupLightProperties();
	
	context->DrawOpaqueRenderList();

	context->DrawSkybox();
}

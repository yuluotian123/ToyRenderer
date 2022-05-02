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
	Scene = SceneManager::getOrCreateInstance()->getCurrentScene();

	return true;
}

void RenderManager::ShutDown()
{
	glfwTerminate();
}

void RenderManager::Render(float DeltaTime)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	context.setupCameraProperties(Rendercamera);

	for (auto model : Scene->getModels()) 
	{
		context.setupModelMatrix(model);

		model->Draw();
	}

	
}

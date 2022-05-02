#include "Engine\Engine.h"
#include "Manager\DisplayManager.h"
#include "Manager\InputManager.h"
#include "Manager\SceneManager.h"
#include "Manager\RenderManager.h"

static double _lastTime = glfwGetTime();

bool Engine::StartUp()
{
	Edisplaymanager = DisplayManager::getOrCreateInstance();
	Escenemanager = SceneManager::getOrCreateInstance();
	Erendermanager = RenderManager::getOrCreateInstance();
	Einputmanager = InputManager::getOrCreateInstance();

	if (!Edisplaymanager->StartUp()) {
		printf( "Display startUp failed." );
		return false;
	}
	EWindow = Edisplaymanager->getWindow();

	if (!Escenemanager->StartUp()) {
		printf("Scene startUp failed.\n");
		return false;
	}

	if (!Erendermanager->StartUp()) {
		printf("Render startUp failed.\n");
		return false;
	}

	if (!Einputmanager->StartUp()) {
		printf("Input startUp failed.\n");
		return false;
	}
	return true;
}

void Engine::ShutDown()
{
	EWindow = nullptr;

	Einputmanager->ShutDown();
	printf("Input ShutDown.\n");

	Erendermanager->ShutDown();
	printf("Render ShutDown.\n");

	Escenemanager->ShutDown();
	printf("Scene ShutDown.\n");

	Edisplaymanager->ShutDown();
	printf("Display ShutDown.\n");
}

void Engine::Update()
{
	while (!glfwWindowShouldClose(EWindow))
	{
		UpdateFrame();

		Einputmanager->processInput(EWindow, deltaTime);
		Escenemanager->Update(deltaTime);       
		Erendermanager->Render(deltaTime);
		Edisplaymanager->Update(deltaTime);

		glfwSwapBuffers(EWindow);
		glfwPollEvents();
	}
}

void Engine::UpdateFrame()
{
	float currentFrame = static_cast<float>(glfwGetTime());

	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

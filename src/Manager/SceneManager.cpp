#include "Manager\SceneManager.h"

bool SceneManager::StartUp()
{
    curSceneID = "ToyRenderer";
    if (!loadScene(curSceneID)) {
        printf("Couldn't Load succesfully.\n");
        return false;
    }
	return true;
}

bool SceneManager::loadScene(std::string sceneID)
{
    curScene = std::make_shared<Scene>(sceneID);

    return !curScene->load_Error;
}

void SceneManager::Update(float DeltaTime)
{
    curScene->Update(DeltaTime);
}


void SceneManager::ShutDown()
{
}
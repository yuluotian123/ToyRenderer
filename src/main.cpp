#include "Engine\Engine.h"
#include <iostream>

int main() 
{
	if (Engine::getOrCreateInstance()->StartUp()) {
		Engine::getOrCreateInstance()->Update();
    }
	else {
		std::cout << "Fail to run the Engine." << std::endl;
	}

	Engine::getOrCreateInstance()->ShutDown();

	return 0;
}
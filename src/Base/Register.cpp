#include "Base\Register.h"
#include "Manager\MaterialSystem.h"

RegisterAction::RegisterAction(const std::string& class_name, FUNC&& generator)
{
	MaterialSystem::getOrCreateInstance()->Register(class_name, std::forward<FUNC>(generator));
}
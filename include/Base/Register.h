#pragma once
#include <string>
#include <memory>
#include <functional>

//materialµÄregister
#define FUNC std::function<std::shared_ptr<void>(void)>
class RegisterAction
{
public:
	RegisterAction(const std::string& class_name, FUNC&& generator);

};

#define REGISTER(CLASS_NAME) \
RegisterAction g_register_action_##CLASS_NAME(#CLASS_NAME, []()\
{\
    return std::make_shared<CLASS_NAME>(); \
});

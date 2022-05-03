#pragma once
#include "OpenGLInterface\Light.h"
#include "Base\Singleton.hpp"

class LightSystem:public Singleton<LightSystem>
{
	friend class Singleton<LightSystem>;
public:
	~LightSystem() {};


private:
	LightSystem() {};

};


#pragma once
#include "glm\glm.hpp"
#include "Base\Common.h"
class Light
{
public:
	std::string type;

	glm::vec3 color = glm::vec3(1.f);
	float intensity = 1.f;

	virtual ~Light(){};
};

class DirectionalLight :public Light {
public:
	glm::vec3 direction = glm::vec3(-1.f);
};

class PointLight :public Light {
public:
	glm::vec3 position = glm::vec3(0.f);
	float radius = 10.f;
};
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Material.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

void Material::setBool(const std::string& name, bool value)
{
	if (!shader || shader->ID == 0)
		printf("You didn't set shader or the shader is invalid.\n");
	else{
		shader->Use();
		shader->setBool(name, value);
	}
}

void Material::setInt(const std::string& name, int value)
{
	if (!shader || shader->ID == 0)
		printf("You didn't set shader or the shader is invalid.\n");
	else {
		shader->Use();
		shader->setInt(name, value);
	}
}

void Material::setFloat(const std::string& name, float value) 
{
	if (!shader || shader->ID == 0)
		printf("You didn't set shader or the shader is invalid.\n");
	else {
		shader->Use();
		shader->setFloat(name, value);
	}
}

void Material::setMat4(const std::string& name, const glm::mat4& mat) 
{
	if (!shader || shader->ID == 0)
		printf("You didn't set shader or the shader is invalid.\n");
	else {
		shader->Use();
		shader->setMat4(name, mat);
	}
	
}

void Material::setVec3(const std::string& name, const glm::vec3& vec) 
{
	if (!shader || shader->ID == 0)
		printf("You didn't set shader or the shader is invalid.\n");
	else {
		shader->Use();
		shader->setVec3(name, vec);
	}
}

void Material::setVec4(const std::string& name, const glm::vec4& vec) 
{
	if (!shader || shader->ID == 0)
		printf("You didn't set shader or the shader is invalid.\n");
	else {
		shader->Use();
		shader->setVec4(name, vec);
	}
}

#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include "Base\Common.h"

//void shaderid::Use() const
//{
//	if (type == SHADERTYPE::SHADER_COMPUTE) return;
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->Use();
//}
//
//void shaderid::Dispatch(unsigned int x, unsigned int y, unsigned int z) const
//{
//	if (type != SHADERTYPE::SHADER_COMPUTE) return;
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->Dispatch(x,y,z);
//}
//
//void shaderid::setBool(const std::string& name, bool value) const
//{
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->setBool(name, value);
//}
//
//void shaderid::setInt(const std::string& name, int value) const
//{
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->setInt(name, value);
//}
//
//void shaderid::setFloat(const std::string& name, float value) const
//{
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->setFloat(name, value);
//}
//
//void shaderid::setMat4(const std::string& name, const glm::mat4& mat) const
//{
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->setMat4(name, mat);
//}
//
//void shaderid::setVec3(const std::string& name, const glm::vec3& vec) const
//{
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->setVec3(name, vec);
//}
//
//void shaderid::setVec4(const std::string& name, const glm::vec4& vec) const
//{
//	MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(ID)->setVec4(name, vec);
//}

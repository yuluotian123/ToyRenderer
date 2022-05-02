#pragma once
#include "OpenGLInterface\Texture.h"
#include "Base\Register.h"
#include <vector>
#include <string>
#include <map>
#include <glm\glm.hpp>

class Shader;
class Material//用子类中声明数据的方式 代替了unordered_map<std::string,std::any> 提升一点点性能的同时 使得各个material的子类的功能更明确
{
public:
	Material(){};

	virtual ~Material() {};

	virtual void UpdateUniform() {};
	virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) {};//考虑在未来去掉这个 改为在渲染器中由用户通过UI加入（可能不会有做到这一步的那一天）
public:
    //给子类使用，但也不是很必要的
	void setShader(std::shared_ptr<Shader> _shader) { shader = _shader; };
    std::shared_ptr<Shader> getShader() { return shader; };

	//给UI界面用
	void setMaterialType(std::string type) { MaterialType = type; };
	const std::string& getMaterialType() { return MaterialType; };
public:
    //可以通过material来设置shader数值 很鸡肋 暂时没有应用场景
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec3(const std::string& name, const glm::vec3& vec);
	void setVec4(const std::string& name, const glm::vec4& vec);
private:
	std::shared_ptr<Shader> shader;

	std::string MaterialType;
};


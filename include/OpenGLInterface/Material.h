#pragma once
#include "OpenGLInterface\Texture.h"
#include "Base\Register.h"
#include "Base\Common.h"
#include <vector>
#include <string>
#include <map>
#include <glm\glm.hpp>
#include "imgui.h"
#include "imgui_stdlib.h"


//用子类中声明数据的方式 代替了unordered_map<std::string,std::any> 提升一点点性能的同时 使得各个material的子类的功能更明确
//下次做材质系统用生成器模式会比较好

class Shader;
class Material
{
public:
	Material(){};

	virtual ~Material() {};

	virtual void UpdateUniform() {};
	virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) {};//考虑在未来去掉这个 改为在渲染器中由用户通过UI加入（可能不会有做到这一步的那一天）
	virtual void ShowMaterialProperties(int id) {};//给ui呈现的material界面 这个暂时还没想好咋写比较好
public:
	void setShader(std::shared_ptr<Shader>& _shader) { shader = _shader; };    //在registerMaterial时使用，其他的时候不要用
    const std::shared_ptr<Shader>& getShader() { return shader; };
	Texture& getDefaultTexture() {
		if (defaultTexture.ID == 0) {
			defaultTexture.loadTexture("./resource/Models/WhiteTexture/WhiteTexture.png", false);
			return defaultTexture;
		}
		else
			return defaultTexture;
	}

	//给UI界面用
	void setMaterialType(std::string type) { MaterialType = type; };
	const std::string& getMaterialType() { return MaterialType; };
private:
	std::shared_ptr<Shader> shader;
	Materialid MaterialType;

	static Texture defaultTexture;
};


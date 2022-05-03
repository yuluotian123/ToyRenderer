#pragma once
#include "OpenGLInterface\Texture.h"
#include "Base\Register.h"
#include <vector>
#include <string>
#include <map>
#include <glm\glm.hpp>
#include "imgui.h"
#include "imgui_stdlib.h"


class Shader;
class Material//���������������ݵķ�ʽ ������unordered_map<std::string,std::any> ����һ������ܵ�ͬʱ ʹ�ø���material������Ĺ��ܸ���ȷ
{
public:
	Material(){};

	virtual ~Material() {};

	virtual void UpdateUniform() {};
	virtual void RegisterMeshData(const std::vector<Texture>& MeshTexture) {};//������δ��ȥ����� ��Ϊ����Ⱦ�������û�ͨ��UI���루���ܲ�����������һ������һ�죩
	virtual void ShowMaterialProperties(int id) {};//��ui���ֵ�material���� �����ʱ��û���զд�ȽϺ�
public:
	void setShader(std::shared_ptr<Shader>& _shader) { shader = _shader; };    //��registerMaterialʱʹ�ã�������ʱ��Ҫ��
    const std::shared_ptr<Shader>& getShader() { return shader; };

	//��UI������
	void setMaterialType(std::string type) { MaterialType = type; };
	const std::string& getMaterialType() { return MaterialType; };
public:
    //����ͨ��material�����ö�Ӧshader��ֵ �ܼ��� ��ʱû��Ӧ�ó���
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


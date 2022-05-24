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


//���������������ݵķ�ʽ ������unordered_map<std::string,std::any> ����һ������ܵ�ͬʱ ʹ�ø���material������Ĺ��ܸ���ȷ
//�´�������ϵͳ��������ģʽ��ȽϺ�

class Shader;
class Material
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
	Texture& getDefaultTexture() {
		if (defaultTexture.ID == 0) {
			defaultTexture.loadTexture("./resource/Models/WhiteTexture/WhiteTexture.png", false);
			return defaultTexture;
		}
		else
			return defaultTexture;
	}

	//��UI������
	void setMaterialType(std::string type) { MaterialType = type; };
	const std::string& getMaterialType() { return MaterialType; };
private:
	std::shared_ptr<Shader> shader;
	Materialid MaterialType;

	static Texture defaultTexture;
};


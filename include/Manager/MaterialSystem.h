#pragma once
#include "Base\Singleton.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <Base\Common.h>

#define FUNC std::function<std::shared_ptr<void>(void)>

class Material;
class Shader;
class MaterialSystem :public Singleton<MaterialSystem>
{
	friend class Singleton<MaterialSystem>;
public:
	~MaterialSystem() {};

	void Register(const std::string& class_name, FUNC&& generator);//ע��material��
	//TO DO: computershader
	//shaderNameΪ�󶨵Ĳ�������
	Shaderid registerShader(const std::string& VPath, const std::string& FPath, const std::string& GPath = "");//���Shader��Shader List
	const Materialid registerMaterial(Shaderid shader, const std::string& materialType);//���ʹ����ע��shader��������materialType��material

	std::shared_ptr<Material> CreateMaterial(const Materialid& id);

	std::unordered_map<Shaderid, std::shared_ptr<Shader>>& getRegisterShaderList() { return ShaderList; };
	std::unordered_map < Materialid, std::vector<std::shared_ptr<Material >>>& getRegisterMaterialList(){ return MaterialList; };

	std::shared_ptr<Shader> getRegisterShaderByID(Shaderid id);
	std::shared_ptr<Shader> getRegisterShaderByMaterialID(const Materialid& id);
private:
	std::pair<std::string, int> ProcessMaterialid(const Materialid& id);//��materialid�����material����+shaderid
private:
	MaterialSystem() {};
	std::unordered_map <std::string, FUNC> MaterialRegister;

	std::unordered_map < Materialid, std::vector<std::shared_ptr<Material >>> MaterialList;	//��ÿ��materialid ����material���飨��Ϊÿ��mesh����Ӧһ��material��
    std::unordered_map<Shaderid,std::shared_ptr<Shader>> ShaderList;//�ƺ�û��Ҫд��sharedptr?
};
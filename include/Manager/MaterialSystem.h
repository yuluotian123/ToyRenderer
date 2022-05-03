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

	void Register(const std::string& class_name, FUNC&& generator);//注册material类
	//TO DO: computershader
	//shaderName为绑定的材质类型
	Shaderid registerShader(const std::string& VPath, const std::string& FPath, const std::string& GPath = "");//添加Shader到Shader List
	const Materialid registerMaterial(Shaderid shader, const std::string& materialType);//添加使用已注册shader和已声明materialType的material

	std::shared_ptr<Material> CreateMaterial(const Materialid& id);

	std::unordered_map<Shaderid, std::shared_ptr<Shader>>& getRegisterShaderList() { return ShaderList; };
	std::unordered_map < Materialid, std::vector<std::shared_ptr<Material >>>& getRegisterMaterialList(){ return MaterialList; };

	std::shared_ptr<Shader> getRegisterShaderByID(Shaderid id);
	std::shared_ptr<Shader> getRegisterShaderByMaterialID(const Materialid& id);
private:
	std::pair<std::string, int> ProcessMaterialid(const Materialid& id);//将materialid处理成material类名+shaderid
private:
	MaterialSystem() {};
	std::unordered_map <std::string, FUNC> MaterialRegister;

	std::unordered_map < Materialid, std::vector<std::shared_ptr<Material >>> MaterialList;	//对每种materialid 储存material数组（因为每个mesh都对应一个material）
    std::unordered_map<Shaderid,std::shared_ptr<Shader>> ShaderList;//似乎没必要写成sharedptr?
};
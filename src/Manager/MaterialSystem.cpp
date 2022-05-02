#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Material.h"
#include "OpenGLInterface\Shader.h"

void MaterialSystem::Register(const std::string& class_name, FUNC&& generator)
{
	MaterialRegister[class_name] = generator;
}

Shaderid MaterialSystem::registerShader(const std::string& VPath, const std::string& FPath, const std::string& GPath)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->setup(VPath, FPath, GPath);
	ShaderList[shader->ID] = shader;
	return shader->ID;
}

const Materialid MaterialSystem::registerMaterial(Shaderid shader,const std::string& materialType)
{
	if (!ShaderList[shader]) { printf("please check if you register this shader.\n"); return ""; }
	
	if(MaterialList.find(materialType + std::to_string(shader))!=MaterialList.end()) {
		printf("You has already registered this material.\n"); 
		return materialType + std::to_string(shader);
	}

	MaterialList[materialType + std::to_string(shader)] = {};

	return materialType + std::to_string(shader);
}

//TO DO:更好的反射
std::shared_ptr<Material> MaterialSystem::CreateMaterial(const Materialid& id)
{
	std::string ClassName = ProcessMaterialid(id).first;
	int sid = ProcessMaterialid(id).second;

	for (auto& x : MaterialRegister)
	{
		if (x.first == ClassName) 
		{
			std::shared_ptr<Material> curM= std::static_pointer_cast<Material>(x.second());
			curM->setShader(ShaderList[sid]);
			curM->setMaterialType(id);
			MaterialList[id].push_back(curM);
			return curM;
		}
	}

	printf("the materialid you use has a wrong materialType or you didn't register this materialid.\n");
	return nullptr;

}

std::pair<std::string, int> MaterialSystem::ProcessMaterialid(const Materialid& id)
{
	int place = 0;
	for (unsigned int i = 0; i < id.size(); i++) {
		if (id[i] >= '0' && id[i] <= '9') {
			place = i;
			break;
		}
	}
    
	std::string classname = id.substr(0, place);
	int shaderid = atoi(id.substr(place , id.size()).c_str());

	return { classname,shaderid };
}
#pragma once
#include <nlohmann\json.hpp>
#include <vector>
#include <string>
#include <memory>

using json = nlohmann::json;
typedef int Shaderid;
typedef std::string Materialid;

class Light;
class Model;
class Camera;
class Scene
{
public:
	Scene(std::string& sceneID);
	~Scene() {};

	void Update(float DeltaTime);

	std::vector<std::shared_ptr<Model>>& getModels(){ return models; };
	std::shared_ptr<Model> getModelByIndex(int index);

	std::vector<std::shared_ptr<Light>>& getLights() { return lights; };

	std::shared_ptr<Camera> fecthOrCreateMainCamera();

	Materialid getMaterialidByName(const std::string& Name);
	Shaderid getShaderidByName(const std::string& Name);
	std::string getMaterialNameByMaterialid(const Materialid Materialid);
	std::string getShaderNameByShaderid(Shaderid shaderid);

	bool load_Error = false;
private:
	bool loadContext();

	void loadShaderandMaterial(json scenejson);
	void loadModels(json scenejson);
	void loadLights(json scenejson);
	void loadCamera(json scenejson);
	void loadSkyBox(json scenejson);
private:
	std::unordered_map<std::string,Shaderid> shaderidList;//��ÿ��ע���shaderid�ҵ�һ��name����Ӧ��������ui������ʾ
	std::unordered_map<std::string,Materialid> materialidList;//��ÿ��ע���materialid�ҵ�һ��name����Ӧ��������ui������ʾ

	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Light>> lights;
	std::shared_ptr<Camera> MainCamera=nullptr;

	std::string m_sceneID;
};


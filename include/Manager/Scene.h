#pragma once
#include "Base\Common.h"
#include <nlohmann\json.hpp>
#include <vector>
#include <string>
#include <memory>

using json = nlohmann::json;

class DirectionalLight;
class Light;
class Model;
class Camera;
class Skybox;
class Scene
{
public:
	Scene(std::string& sceneID);
	~Scene() {};

	void Update(float DeltaTime);

	std::vector<std::shared_ptr<Model>>& getModels(){ return models; };
	std::shared_ptr<Model> getModelByIndex(int index);

	std::vector<std::shared_ptr<Light>>& getLights() { return lights; };
	std::shared_ptr <DirectionalLight> getMainLight();

	std::shared_ptr<Skybox> getSkybox() { return MainSkybox; };

	std::shared_ptr<Camera> getMainCamera() { return MainCamera; };

	Materialid getMaterialidByName(const std::string& Name);
	Shaderid getShaderidByName(const std::string& Name);
	std::string getMaterialNameByMaterialid(const Materialid Materialid);
	std::string getShaderNameByShaderid(Shaderid shaderid);

	void addModel(const std::string& path, const Materialid& materialid, glm::vec3& position, glm::vec3& scalling, glm::vec3& rotationAxis, float angle);
	void deleteModel(std::shared_ptr<Model> model);

	void addLight(glm::vec3 position, float radius, glm::vec3 color, float intensity);

	bool load_Error = false;
private:
	bool loadContext();

	void loadShaderandMaterial(json scenejson);
	void loadModels(json scenejson);
	void loadLights(json scenejson);
	void loadCamera(json scenejson);
	void loadSkyBox(json scenejson);
private:
	std::unordered_map<std::string,Shaderid> shaderidList;//将每个用户注册的shaderid找到一个name来对应，用来在ui界面显示
	std::unordered_map<std::string,Materialid> materialidList;//将每个用户注册的materialid找到一个name来对应，用来在ui界面显示

	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Light>> lights;
	std::shared_ptr<Camera> MainCamera = nullptr;
	std::shared_ptr<Skybox> MainSkybox = nullptr;

	std::string m_sceneID;
};


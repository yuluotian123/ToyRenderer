#include "OpenGLInterface\Material.h"
#include "Manager\Scene.h"
#include "OpenGLInterface\Camera.h"
#include "OpenGLInterface\Model.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Skybox.h"
#include <iostream>
#include <fstream>

//TO DO: �����л�����������
Scene::Scene(std::string& sceneID)
{
    m_sceneID = sceneID;

    load_Error = !loadContext();
}

bool Scene::loadContext()
{
    std::string folderPath = "resource/Scenes/";
    std::string fileExtension = ".json";
    std::string scenePath = folderPath + m_sceneID + fileExtension;
    std::ifstream file(scenePath.c_str());

    if (!file.good()) {
        printf("The config file: %s is error.\n", scenePath.c_str());
        return false;
    }

    json sceneJson;
    file >> sceneJson;

    if (sceneJson["sceneID"] != m_sceneID & ((unsigned int)sceneJson["models"].size() != 0)) {
        printf("The config file: %s does not belong to current scene.\n",scenePath.c_str());
        return false;
    }

    printf("Begin to load scene: %s.\n", m_sceneID.c_str());
    printf("Loading camera...\n");
    loadCamera(sceneJson);

    printf("Loading shaders and materials...\n");
    loadShaderandMaterial(sceneJson);

    printf("Loading models...\n");
    loadModels(sceneJson);

    printf("Loading skybox...\n");
    loadSkyBox(sceneJson);

    printf("Loading lights...\n");
    loadLights(sceneJson);

    return true;
}

void Scene::loadCamera(json scenejson)
{
    //TO DO����json�趨����
    json camerasjson = scenejson["camera"];
    for (unsigned i = 0; i < camerasjson.size(); i++) {
        json camera = camerasjson[i];
        glm::vec3 position = glm::vec3((float)camera["Position"][0], (float)camera["Position"][1], (float)camera["Position"][2]);
        glm::vec3 up = glm::vec3((float)camera["Up"][0], (float)camera["Up"][1], (float)camera["Up"][2]);
        float yaw = camera["Yaw"];
        float pitch = camera["Pitch"];
        float nearp = camera["NearPlane"];
        float farp = camera["FarPlane"];
        MainCamera = std::make_shared<Camera>(position,up,yaw,pitch,nearp,farp);
    }
}

void Scene::loadShaderandMaterial(json scenejson)
{
    json shadersjson = scenejson["shaders"];
    json materials = scenejson["materials"];

    for (unsigned int i = 0; i < shadersjson.size(); i++) {
        json shader = shadersjson[i];
        bool computer = shader["IsComputer"];
        if (!computer) {
            Shaderid shaderid = MaterialSystem::getOrCreateInstance()->registerShader(shader["VertexPath"], shader["FragementPath"], shader["GeometryPath"]);
            if (shaderidList.find(shader["Name"]) == shaderidList.end())
                shaderidList[shader["Name"]] = shaderid;
            else
                printf("You has already register a shader named the same.\n");
        }
    }

    for (unsigned int i = 0; i < materials.size(); i++) {
        json material = materials[i];
        Materialid materialid = MaterialSystem::getOrCreateInstance()->registerMaterial(shaderidList[material["UsingShader"]], material["Type"]);
        if (materialidList.find(material["Name"]) == materialidList.end()) {
            if (getMaterialNameByMaterialid(materialid) == "")
                materialidList[material["Name"]] = materialid;
            else
                printf("You has already register a same materialType.\n");
        }
        else
            printf("You has already register a material named the same.\n");
    }
}

void Scene::loadModels(json scenejson)
{
    Transform initTrans;
    json modelsjson = scenejson["models"];
    for (unsigned int i = 0; i < modelsjson.size(); i++) {
        json model = modelsjson[i];
        initTrans.translation = glm::vec3((float)model["Position"][0], (float)model["Position"][1], (float)model["Position"][2]);
        initTrans.scaling = glm::vec3((float)model["Scaling"][0], (float)model["Scaling"][1], (float)model["Scaling"][2]);
        initTrans.angle = glm::radians((float)model["Rotation"] [0] );
        initTrans.rotationAxis= glm::vec3((float)model["Rotation"][1], (float)model["Rotation"][2], (float)model["Rotation"][3]);
        std::shared_ptr<Model> curmodel=std::make_shared<Model>(model["Path"], initTrans);
        if (model["Material"] != "Null"&&model["Material"]!="")
            curmodel->SetMaterials(materialidList[model["Material"]]);
        models.push_back(curmodel);
    }
}

void Scene::loadSkyBox(json scenejson)
{
    CubeMap::cubeMapCube.setupMesh();
    json skyboxjson = scenejson["skybox"];
    if (skyboxjson.size() > 0) {
        std::string path = skyboxjson[0]["Path"];
        bool ishdr = skyboxjson[0]["Hdr"];
        unsigned int resolution = skyboxjson[0]["Resolution"];
        MainSkybox = std::make_shared<Skybox>();
        MainSkybox->setup(path, ishdr, resolution);
    }

}

void Scene::loadLights(json scenejson)
{
    json lightsjson = scenejson["lights"];
    for (unsigned int i = 0; i < lightsjson.size(); i++) {
        json light = lightsjson[i];
        if (light["Type"] == "DirectionalLight") {
            std::shared_ptr<DirectionalLight> dirLight = std::make_shared<DirectionalLight>();
            dirLight->type = light["Type"];
            dirLight->color = glm::vec3((float)light["Color"][0], (float)light["Color"][1], (float)light["Color"][2]);
            dirLight->intensity = (float)light["Intensity"];
            dirLight->direction= glm::vec3((float)light["Direction"][0], (float)light["Direction"][1], (float)light["Direction"][2]);

            lights.push_back(dirLight);
        }
        if (light["Type"] == "PointLight") {
            std::shared_ptr<PointLight> pLight = std::make_shared<PointLight>();
            pLight->type = light["Type"];
            pLight->color = glm::vec3((float)light["Color"][0], (float)light["Color"][1], (float)light["Color"][2]);
            pLight->intensity = (float)light["Intensity"];
            pLight->position = glm::vec3((float)light["Position"][0], (float)light["Position"][1], (float)light["Position"][2]);
            pLight->radius = light["Radius"];
            lights.push_back(pLight);
        }
    }
}

void Scene::Update(float DeltaTime)
{
    for (auto model : models)
        model->Update(DeltaTime);
}

std::shared_ptr<Model> Scene::getModelByIndex(int index)
{
    if (index > models.size()) { printf("can't find model at index:%d.return null.\n",index);  return nullptr; }
    return models[index];
}

Materialid Scene::getMaterialidByName(const std::string& Name)
{
    if (materialidList.find(Name) != materialidList.end())
        return materialidList[Name]; 
    else
        return "";
}

Shaderid Scene::getShaderidByName(const std::string& Name)
{
    if (shaderidList.find(Name) != shaderidList.end())
        return shaderidList[Name];
    else
        return -1;
}

std::string Scene::getMaterialNameByMaterialid(const Materialid materialid)
{
    for (auto Mpair : materialidList) {
        if (Mpair.second == materialid)
            return Mpair.first;
    }

    return "";
}

std::string Scene::getShaderNameByShaderid(Shaderid shaderid)
{
    for (auto Spair : shaderidList) {
        if (Spair.second == shaderid)
            return Spair.first;
    }

    return "";
}

void Scene::addModel(const std::string& path, const Materialid& materialid, glm::vec3& position, glm::vec3& scalling, glm::vec3& rotationAxis, float angle)
{
    Transform trans;
    trans.translation = position;
    trans.scaling = scalling;
    trans.angle = angle;
    trans.rotationAxis = rotationAxis;
    std::shared_ptr<Model> model = std::make_shared<Model>(path, trans);
    if (!(materialid == "" || materialid == "Null"))
        model->SetMaterials(getMaterialidByName(materialid));
    models.push_back(model);
}

void Scene::deleteModel(std::shared_ptr<Model> model)
{
    for (auto mesh : model->getMeshes()) {
        auto& map = MaterialSystem::getOrCreateInstance()->getRegisterMaterialList();

        for (auto it = map[mesh->getMaterial()->getMaterialType()].begin(); it != map[mesh->getMaterial()->getMaterialType()].end(); it++) {
            if (*it == mesh->getMaterial())
            {
                map[mesh->getMaterial()->getMaterialType()].erase(it);
                break;
            }
        }
    }

    for (auto it = models.begin(); it != models.end(); it++) {
        if (*it == model)
        {
            models.erase(it);
            break;
        }
    }
}

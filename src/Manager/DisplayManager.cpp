#include "Manager\DisplayManager.h"
#include "Manager\SceneManager.h"
#include "Manager\MaterialSystem.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "Base\Common.h"
#include "OpenGLInterface\Camera.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Material.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Light.h"
#include "Passes\ClusterLight.h"

bool DisplayManager::StartUp()
{
    if(!CreateWindow())
        return false;
    if (!CreateImguiContext())
        return false;

    return true;
}

void DisplayManager::Update(float deltaTime)
{
    if(!scene)
        scene = SceneManager::getOrCreateInstance()->getCurrentScene();
    if(!camera) 
        camera = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainCamera();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool showCameraControl = true;
    static bool showSceneList = true;
    static bool showDemoWindow = false;
    static bool showMaterialSystem = true;

    {
        ImGui::Begin("Toy Renderer"); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Checkbox("Show Demo Window:", &showDemoWindow);

        ImGui::Checkbox("Show Camera Control:", &showCameraControl);

        ImGui::Checkbox("Show Scene Properties:", &showSceneList);

        ImGui::Checkbox("Show MaterialSystem:", &showMaterialSystem);
        ImGui::End();
    }

    if (showCameraControl)
        ShowCameraControl();

    if (showSceneList)
        ShowSceneList();

    if(showDemoWindow)
        ImGui::ShowDemoWindow();

    if (showMaterialSystem)
        ShowMaterialSystem();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DisplayManager::ShowCameraControl()
{
    ImGui::Begin("Controls");
    ImGui::Text("Strafe: W A S D");
    ImGui::Text("Rotate Camera: hold right click + mouse");
    ImGui::Text("Up&Down: Q E");
    ImGui::Text("Exit: ESC");
    ImGui::InputFloat3("Camera Pos", (float*)camera->GetPositionp());
    ImGui::SliderFloat("Movement speed", camera->GetSpeedp(), 0.005f, 100.0f);
    ImGui::SliderFloat("NearPlane", camera->GetNearPlanep(), 0.1f, camera->GetFarPlane());
    ImGui::SliderFloat("FarPlane", camera->GetFarPlanep(),camera->GetNearPlane(), 3000.f);
    ImGui::End();
}

void DisplayManager::ShowSceneList()
{
    ImGui::Begin("Properties:");

    if (ImGui::CollapsingHeader("Models"))
        ShowModelList();

    if (ImGui::CollapsingHeader("Lights"))
        ShowLightList();

    ImGui::End();
}

void DisplayManager::ShowMaterialSystem()
{
    ImGui::Begin("MaterialSystem:");
     
    if (ImGui::CollapsingHeader("Shaders")) {
        int i = 0;
        for (auto& spair : MaterialSystem::getOrCreateInstance()->getRegisterShaderList()) {
            
            ImGui::PushID(i);
            bool node_open = ImGui::TreeNode(scene->getShaderNameByShaderid(spair.first).c_str());
            if (node_open) {
                if (spair.second->CPath != "") {
                    ImGui::Text("ShaderID: %d", spair.first);
                    ImGui::Text("ComputePath:%s", spair.second->CPath.c_str());
                    ImGui::TreePop();
                }
                else {
                    ImGui::Text("ShaderID: %d", spair.first);
                    ImGui::Text("VertexPath:%s", spair.second->VPath.c_str());
                    ImGui::Text("FragementPath:%s", spair.second->FPath.c_str());
                    ImGui::Text("GeometryPath:%s", spair.second->GPath.c_str());
                    ImGui::TreePop();
                }
            }
            ImGui::PopID();
            i++;
        }
    }

    if (ImGui::CollapsingHeader("Materials")) {
        int j = 0;
        for (auto mpair : MaterialSystem::getOrCreateInstance()->getRegisterMaterialList()) {
            ImGui::PushID(j*5000);
            bool node_open = ImGui::TreeNode(scene->getMaterialNameByMaterialid(mpair.first).c_str());
            if (node_open) {
                for (unsigned int m = 0; m < mpair.second.size(); m++) {
                    ImGui::Text("Index: %d", m);
                    ImGui::Text("MaterialType: %s", mpair.second[m]->getMaterialType().c_str());
                    ImGui::Text("ShaderName: %s", scene->getShaderNameByShaderid(mpair.second[m]->getShader()->ID).c_str());
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
            j++;
        }
    }

    ImGui::End();
}

void DisplayManager::ShowModelList()
{
    static bool addmodel = false;
    if (ImGui::BeginTable("Models", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
        for (unsigned int i = 0; i < scene->getModels().size(); i++)
            ShowModels(i);

        ImGui::EndTable();
    }

    if (ImGui::Button("Add Model"))
        addmodel = true;

    if (addmodel) AddModel(&addmodel);
}

void DisplayManager::ShowLightList()
{
    ClusterLight::Update = false;

    static bool addlight = false;
    if (addlight) addLight(&addlight);
    if (ImGui::BeginTable("Lights", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
        for (unsigned int i = 0; i < scene->getLights().size(); i++) {
            std::shared_ptr<Light> light = scene->getLights()[i];
            ImGui::PushID(i);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            bool node_open = ImGui::TreeNode((light->type + std::to_string(i)).c_str());
            if (node_open) {
                ImGui::PushID(i + 1000);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Intensity");
                ImGui::TableSetColumnIndex(1);
                ImGui::AlignTextToFramePadding();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat("##value", (float*)&light->intensity);
                ImGui::PopID();

                ImGui::PushID(i + 2000);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Color");
                ImGui::TableSetColumnIndex(1);
                ImGui::AlignTextToFramePadding();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat3("##value", (float*)&light->color);
                ImGui::PopID();

                if (light->type == "DirectionalLight") {
                    ImGui::PushID(i + 3000);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Direction");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::AlignTextToFramePadding();
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::InputFloat3("##value", (float*)&std::dynamic_pointer_cast<DirectionalLight>(light)->direction);
                    ImGui::PopID();
                }
                else if (light->type == "PointLight") {
                    ImGui::PushID(i + 3000);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Radius");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::AlignTextToFramePadding();
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::InputFloat("##value", (float*)&std::dynamic_pointer_cast<PointLight>(light)->radius);
                    ImGui::PopID();

                    ImGui::PushID(i + 4000);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Position");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::AlignTextToFramePadding();
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::InputFloat3("##value", (float*)&std::dynamic_pointer_cast<PointLight>(light)->position);
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    if (ImGui::Button("Add Light")) {
        addlight = true;
    }
    if (ImGui::Button("Update")) {
        ClusterLight::Update = true;
    }
}

void DisplayManager::addLight(bool* open)
{
    ImGui::Begin("Add Light");

    static glm::vec3 position{ 0.f };
    static glm::vec3 color{ 1.f };
    static float radius = 10.f;
    static float intensity = 10.f;

    ImGui::InputFloat3("Position", (float*)&position);
    ImGui::InputFloat3("Color", (float*)&color);
    ImGui::InputFloat("Radius", &radius);
    ImGui::InputFloat("Intensity", &intensity);
    if (ImGui::Button("Add"))
    {
        scene->addLight(position,radius,color,intensity);
        *open = false;
    }

    ImGui::End();
}

void DisplayManager::AddModel(bool* open)
{
    ImGui::Begin("Add Model");

    static glm::vec3 position{ 0.f };
    static glm::vec3 scalling{ 1.f };
    static glm::vec3 rotationAxis{ 1.f };
    static float angle = 0.f;

    static std::string path = "C:/Users/ylt195/source/repos/openGL/resource/model/marry/Marry.obj";
    static std::string material = "";

    ImGui::InputFloat3("Position", (float*)&position);
    ImGui::InputFloat3("Scalling", (float*)&scalling);
    ImGui::InputFloat3("RotationAxis", (float*)&rotationAxis);
    ImGui::InputFloat("Angle", (float*)&angle);
    ImGui::InputText("Path", &path);
    ImGui::InputText("MaterialName",&material);

    if (ImGui::Button("Add"))
    {
        scene->addModel(path, material, position, scalling, rotationAxis, angle);
        *open = false;
    }

    ImGui::End();
}

void DisplayManager::ShowModels(int id)
{
    std::shared_ptr<Model> curmodel = scene->getModelByIndex(id);
    ImGui::PushID(id);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode(curmodel->getDirectory().substr(curmodel->getDirectory().find_last_of('/') + 1).c_str());
    ImGui::TableSetColumnIndex(1);
    if (ImGui::Button("Delete Model"))
    {
        DeleteModel(curmodel);
    }
    if (node_open) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
    
		ImGui::PushID(-1 * (id+1));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Position", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputFloat3("##value", (float*)curmodel->getPositionp());
		ImGui::NextColumn();
		ImGui::PopID();

		ImGui::PushID(-2 * (id + 1));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Scalling", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputFloat3("##value", (float*)curmodel->getScallingp());
		ImGui::NextColumn();
		ImGui::PopID();

		ImGui::PushID(-3 * (id + 1));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("RotationAxis", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputFloat3("##value", (float*)curmodel->getRotationAxisp());
		ImGui::NextColumn();
		ImGui::PopID();

		ImGui::PushID(-4 * (id + 1));
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("Angle", flags);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputFloat("##value", (float*)curmodel->getAnglep());
		ImGui::NextColumn();
		ImGui::PopID();

        for (unsigned int i = 0; i < curmodel->getMeshes().size(); i++) {
            ShowMeshes(curmodel->getMeshbyIndex(i), i,id);
        }
        
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void DisplayManager::DeleteModel(std::shared_ptr<Model> model)
{
    scene->deleteModel(model);
}

void DisplayManager::ShowMeshes(std::shared_ptr<Mesh> mesh,int id, int modelid)
{
    static int RESET = -1;
    ImGui::PushID(id);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Mesh","Mesh_%d",id);
    ImGui::TableSetColumnIndex(1);
    if (ImGui::Button("Reset")) RESET = (id+1)*(modelid*100+1);
    if (node_open) {
        //能够更改材质参数 还没想好怎么写
        if (mesh->getMaterial())
        mesh->getMaterial()->ShowMaterialProperties((id * 100 + 1) * (modelid*100+1));
        ImGui::TreePop();
    }

    if (RESET == (id + 1) * (modelid*100 + 1)) ResetMaterial(mesh, &RESET);

    ImGui::PopID();
}

void DisplayManager::ResetMaterial(std::shared_ptr<Mesh> mesh,int* open)
{
    ImGui::Begin("Reset Material");

    static std::string MaterialName="";

    ImGui::InputText("MaterialName",&MaterialName);

    if (ImGui::Button("Reset")) {
        if (MaterialName != "") {
            mesh->setMaterial(scene->getMaterialidByName(MaterialName));
            *open = -1;
        }
        else {
            *open = -1;
        }
    }

    ImGui::End();
}

void DisplayManager::ShutDown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
}

GLFWwindow* DisplayManager::getWindow() const
{
    return m_window;
}

bool DisplayManager::CreateWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ToyRenderer", NULL, NULL);
    if (m_window == nullptr)
    {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD.\n");
        return false;
    }
    return true;
}

bool DisplayManager::CreateImguiContext()
{
    ImGuiContext* mGuiContext = ImGui::CreateContext();     // Setup Dear ImGui context
    if (mGuiContext == nullptr)
        return false;
    ImGui::StyleColorsDark();       // Setup Dear ImGui style
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);     // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init("#version 450");

    return true;
}
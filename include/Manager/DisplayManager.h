#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Base\Singleton.hpp"
#include <string>

class Camera;
class Scene;
class Mesh;
class Model;
class Material;
//��ʾui���棬��ʾ���ڣ�����������
class DisplayManager:public Singleton<DisplayManager>
{
	friend class Singleton<DisplayManager>;
public:
	~DisplayManager() {};

	bool StartUp();
	void Update(float deltaTime);
	void ShutDown();

	GLFWwindow* getWindow() const;
private:
	void ShowCameraControl();
	void ShowSceneList();
	void ShowMaterialSystem();

	//showSceneList���ӷ������Ժ���ܻᵥ������һ���ࣩ
	void ShowModelList();
	void AddModel(bool* open);//���Ʊ�ը�����ܣ�loadModel��
	void DeleteModel(std::shared_ptr<Model> model);
	void ShowModels(int i);
	void ShowMeshes(std::shared_ptr<Mesh> mesh,int id,int modelid);
	void ResetMaterial(std::shared_ptr<Mesh> mesh,int* open);

	void ShowLightList();
private:
	bool CreateWindow();
	bool CreateImguiContext();

	GLFWwindow* m_window = nullptr;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Scene> scene;
private:
	DisplayManager() {};

};


#pragma once
#include "OpenGLInterface\RenderTarget.h"
#include <memory>
#include <vector>
#include "Base\common.h"

class Camera;
class Model;
class Light;
class Scene;
namespace CameraUniformNameList {//��Ϊʲôc++û�о�̬�ࣿ��
	const std::string view = "ViewMatrix";
	const std::string projection = "ProjectionMatrix";
	const std::string invView = "InvViewMatrix";
	const std::string invProjection = "InvProjectionMatrix";
	const std::string CamPos = "CameraPos";
	const std::string model = "ModelMatrix";
	const std::string Scrwidth = "ScreenWidth";
	const std::string Scrheight = "ScreenHeight";
};

//ΪʲôҪ��rendermanager���ﴫ����������أ� ��Ϊ�Ժ���ܻ���һ�������������
class RenderContext
{
public:
	bool Init();

	void DrawOpaqueRenderList();
	void DrawOpaqueRenderList(Shaderid shaderid);

	void DrawSkybox();

	void setupCameraProperties(std::shared_ptr<Camera> camera);
private:
	void setupModelMatrix(std::shared_ptr<Model> model);//���ǽ��˷�������Model->Draw

	void InitSkyboxforIBL();

private:
	std::shared_ptr<Scene> curScene = nullptr;

	Shaderid EquiRecToCubeshader, irradianceShader,specFilterShader, SkyBoxshader;

	RenderTarget EquiRecToCubeRT,irradianceRT,specFilterRT;

	CubeMap irradianceMap, specFilteredMap;

	Texture brdfLUT;

};
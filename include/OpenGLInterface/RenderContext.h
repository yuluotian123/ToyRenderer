#pragma once
#include "OpenGLInterface\RenderTarget.h"
#include <memory>
#include <vector>
#include "Base\common.h"

class Camera;
class Model;
class Light;
class Scene;
namespace CameraUniformNameList {//（为什么c++没有静态类？）
	const std::string view = "ViewMatrix";
	const std::string projection = "ProjectionMatrix";
	const std::string invView = "InvViewMatrix";
	const std::string invProjection = "InvProjectionMatrix";
	const std::string CamPos = "CameraPos";
	const std::string model = "ModelMatrix";
	const std::string Scrwidth = "ScreenWidth";
	const std::string Scrheight = "ScreenHeight";
};

//为什么要从rendermanager那里传入相机数据呢？ 因为以后可能会有一个场景多相机！
class RenderContext
{
public:
	bool Init();

	void DrawOpaqueRenderList();
	void DrawOpaqueRenderList(Shaderid shaderid);

	void DrawSkybox();

	void setupCameraProperties(std::shared_ptr<Camera> camera);
private:
	void setupModelMatrix(std::shared_ptr<Model> model);//考虑将此方法移入Model->Draw

	void InitSkyboxforIBL();

private:
	std::shared_ptr<Scene> curScene = nullptr;

	Shaderid EquiRecToCubeshader, irradianceShader,specFilterShader, SkyBoxshader;

	RenderTarget EquiRecToCubeRT,irradianceRT,specFilterRT;

	CubeMap irradianceMap, specFilteredMap;

	Texture brdfLUT;

};
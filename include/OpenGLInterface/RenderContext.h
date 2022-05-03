#pragma once
#include <memory>
#include <vector>
#include "Base\common.h"

class Camera;
class Model;
class Light;
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

class RenderContext
{
public:
	void DrawOpaqueRenderList(const std::vector<std::shared_ptr<Model>>& models);
	void DrawOpaqueRenderList(const std::vector<std::shared_ptr<Model>>& models,Shaderid shaderid);

	void setupCameraProperties(std::shared_ptr<Camera> camera);

	void setupModelMatrix(std::shared_ptr<Model> model);//考虑将此方法移入Model->Draw

	void setupLightProperties(const std::vector<std::shared_ptr<Light>>& lights);

};
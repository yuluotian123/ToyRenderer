#pragma once
#include "BasePass.h"

class DirectionalLight;
class CSMshadowMap :
    public BasePass
{
public:
	CSMshadowMap(const std::string& PassName, int Order)
		:BasePass(PassName, Order) {};

	void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
	void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
private:
	void updateCameraFrustum(std::shared_ptr<Camera>& Rendercamera, glm::vec3 lightDir);

	void updateDistanceList(std::shared_ptr<Camera>& Rendercamera);
	void updateLightViewMat(std::shared_ptr<Camera>& Rendercamera, glm::vec3 lightDir);
	void updateClipPointByCamera(std::shared_ptr<Camera>& Rendercamera);
	void updateShadowMatUBO();
private:
	Shaderid shadowMapShader;
	std::shared_ptr<DirectionalLight> mainLight;
	RenderTarget shadowArrayFBO;
	unsigned ShadowMatUBO;

	const int Split = 5;
	const unsigned resolution = 2048;

	std::vector<glm::mat4>  LightProViewMatList;
	std::vector<float> FarPlaneDistantList;
	std::vector<std::vector<glm::vec4>> ClipPointList;
};


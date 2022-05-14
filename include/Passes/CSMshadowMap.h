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
	void upadteCameraFrustum(std::shared_ptr<Camera>& Rendercamera, glm::vec3 lightDir);
private:
	Shaderid shadowMapShader;
	std::shared_ptr<DirectionalLight> mainLight;
	RenderTarget shadowArrayFBO;

	const int Split = 5;
	const unsigned resolution = 1024;
};


#pragma once
#include "BasePass.h"
class CSMshadowMap :
    public BasePass
{
public:
	CSMshadowMap(const std::string& PassName, int Order)
		:BasePass(PassName, Order) {};

	void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
	void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
private:
	Shaderid shadowMapShader;
};


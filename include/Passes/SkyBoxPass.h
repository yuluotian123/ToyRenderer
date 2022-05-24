#pragma once
#include "OpenGLInterface\RenderTarget.h"
#include "BasePass.h"
class SkyBoxPass :
    public BasePass
{
public:
	SkyBoxPass(const std::string& PassName, int Order)
		:BasePass(PassName, Order) {};

	void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
	void lateInit(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
	void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;

private:
	Shaderid EquiRecToCubeshader, irradianceShader, specFilterShader, SkyBoxshader;

	RenderTarget EquiRecToCubeRT, irradianceRT, specFilterRT;
	CubeMap irradianceMap, specFilteredMap;
	Texture brdfLUT;
};


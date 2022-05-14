#include "Passes\CSMshadowMap.h"
#include "Manager\MaterialSystem.h"

void CSMshadowMap::init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	shadowMapShader = MaterialSystem::getOrCreateInstance()->registerShader("CSMshadowmapVs.glsl", "CSMshadowmapFs.glsl");
}

void CSMshadowMap::update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
}

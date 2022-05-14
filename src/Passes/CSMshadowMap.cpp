#include "Passes\CSMshadowMap.h"
#include "Manager\SceneManager.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Camera.h"

void CSMshadowMap::init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	shadowMapShader = MaterialSystem::getOrCreateInstance()->registerShader("CSMshadowmapVs.glsl", "CSMshadowmapFs.glsl");
	shadowArrayFBO.createRenderTarget(resolution, resolution, RenderTarget::ENUM_TYPE_DEPTH_ARRAY, Split);
	
}

void CSMshadowMap::update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	mainLight = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainLight();
	upadteCameraFrustum(Rendercamera, mainLight->direction);


}

void CSMshadowMap::upadteCameraFrustum(std::shared_ptr<Camera>& Rendercamera, glm::vec3 lightDir)
{
	glm::vec3 camPos = Rendercamera->GetPosition();
	glm::vec3 camDir = Rendercamera->GetFront();
	glm::vec3 camUp = Rendercamera->

}

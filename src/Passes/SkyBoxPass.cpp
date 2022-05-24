#include "Passes\SkyBoxPass.h"
#include "Manager\MaterialSystem.h"
#include "Manager\SceneManager.h"
#include "Manager\Scene.h"
#include "OpenGLInterface\Skybox.h"
#include "OpenGLInterface\Shader.h"

void SkyBoxPass::init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);//让立方体贴图边缘平滑

	std::shared_ptr<Skybox> skybox = SceneManager::getOrCreateInstance()->getCurrentScene()->getSkybox();

	if (skybox == nullptr) return;

	irradianceMap.createCubeMap(32, 32, CUBEMAPTYPE::HDR_MAP);
	specFilteredMap.createCubeMap(256, 256, CUBEMAPTYPE::PREFILTER_MAP);

	brdfLUT.loadHDRTexture("G:/gitHubGameProject/ToyRenderer/resource/Skybox/ibl_brdf_lut.png");

	EquiRecToCubeRT.createRenderTarget(skybox->getResolution(), skybox->getResolution(), RenderTarget::ENUM_TYPE_CAPTURE);

	SkyBoxshader = MaterialSystem::getOrCreateInstance()->registerShader("skyBoxVs.glsl", "skyBoxFs.glsl");
	EquiRecToCubeshader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl", "EquiRectangularToCubeFs.glsl");
	irradianceShader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl", "irradianceFs.glsl");
	specFilterShader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl", "preFilterFs.glsl");

	EquiRecToCubeRT.use();
	if (skybox->IsHDR())
		skybox->fillCubeMapWithHDR(EquiRecToCubeshader);

	EquiRecToCubeRT.resizeforCapture(irradianceMap.width, irradianceMap.height);
	irradianceMap.convolveCubeMap(skybox->getTextureID(), irradianceShader);

	EquiRecToCubeRT.resizeforCapture(specFilteredMap.width, specFilteredMap.height);
	specFilteredMap.preFilterCubeMap(skybox->getTextureID(), EquiRecToCubeRT.GetRBO(), specFilterShader);

	EquiRecToCubeRT.UseDefault();

}

void SkyBoxPass::lateInit(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	//需要放在最后才行，不然不能覆盖所有shader
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		//绑定这三张图片在指定的3个位置（直接每帧更新是不是不太好）
		//全局的tex应该怎么绑定比较好呢？
		shaderP.second->Use();
		shaderP.second->setInt("irradianceMap", RenderManager::getCurGlobalTexNum());
		shaderP.second->setInt("specularMap", RenderManager::getCurGlobalTexNum() + 1);
		shaderP.second->setInt("brdfLUT", RenderManager::getCurGlobalTexNum() + 2);
	}

	glActiveTexture(GL_TEXTURE0 + RenderManager::getCurGlobalTexNum());
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap.ID);

	glActiveTexture(GL_TEXTURE0 + RenderManager::getCurGlobalTexNum() + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, specFilteredMap.ID);

	glActiveTexture(GL_TEXTURE0 + RenderManager::getCurGlobalTexNum() + 2);
	glBindTexture(GL_TEXTURE_2D, brdfLUT.ID);

	RenderManager::addCurGlobalTexNum(3);
}

void SkyBoxPass::update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	glEnable(GL_DEPTH_TEST);
	std::shared_ptr<Skybox> skybox = SceneManager::getOrCreateInstance()->getCurrentScene()->getSkybox();
	std::shared_ptr<Shader> shader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(SkyBoxshader);
	shader->Use();
	shader->isMaterial = true;
	shader->setInt("environmentMap", 0);
	skybox->draw();
	glDisable(GL_DEPTH_TEST);
}

#include "OpenGLInterface\RenderTarget.h"
#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "Manager\SceneManager.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Camera.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Skybox.h"

bool RenderContext::Init()
{
	curScene = SceneManager::getOrCreateInstance()->getCurrentScene();
	SkyBoxshader = MaterialSystem::getOrCreateInstance()->registerShader("skyBoxVs.glsl", "skyBoxFs.glsl");

	if (curScene->getSkybox() && curScene->getSkybox()->IsHDR())
		InitSkyCubeMapFromHDR();

	return true;
}

void RenderContext::DrawOpaqueRenderList()
{
	for (auto& model : curScene->getModels()) {
		setupModelMatrix(model);
		model->Draw();
	}
}

void RenderContext::DrawOpaqueRenderList(Shaderid shaderid)
{
	for (auto& model : curScene->getModels()) {
		setupModelMatrix(model);
		model->Draw(shaderid);
	}
}

void RenderContext::InitSkyCubeMapFromHDR()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);//让立方体贴图边缘平滑

	std::shared_ptr<Skybox> skybox = curScene->getSkybox();

	irradianceMap.createCubeMap(32, 32, CUBEMAPTYPE::HDR_MAP);
	specFilteredMap.createCubeMap(256, 256, CUBEMAPTYPE::PREFILTER_MAP);

	brdfLUT.loadHDRTexture("G:/gitHubGameProject/ToyRenderer/resource/Skybox/ibl_brdf_lut.png");

	EquiRecToCubeRT.createRenderTarget(skybox->getResolution(), skybox->getResolution(), RenderTarget::ENUM_TYPE_CAPTURE);

	EquiRecToCubeshader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl", "EquiRectangularToCubeFs.glsl");
	irradianceShader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl","irradianceFs.glsl");
	specFilterShader = MaterialSystem::getOrCreateInstance()->registerShader("EquiRectangularToCubeVs.glsl", "preFilterFs.glsl");

	EquiRecToCubeRT.use();
	skybox->fillCubeMapWithHDR(EquiRecToCubeshader);

	EquiRecToCubeRT.resizeforCapture(irradianceMap.width, irradianceMap.height);
	irradianceMap.convolveCubeMap(skybox->getTextureID(), irradianceShader);

	EquiRecToCubeRT.resizeforCapture(specFilteredMap.width, specFilteredMap.height);
	specFilteredMap.preFilterCubeMap(skybox->getTextureID(), EquiRecToCubeRT.GetRBO(), specFilterShader);

	EquiRecToCubeRT.UseDefault();

	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		//绑定这三张图片在指定的3个位置（直接每帧更新是不是不太好）
		//全局的tex应该怎么绑定比较好呢？
		if (shaderP.second->useIBL) {
			shaderP.second->Use();
			shaderP.second->setInt("irradianceMap", 10);
			shaderP.second->setInt("specularMap", 11);
			shaderP.second->setInt("brdfLUT", 12);
		}
	}

	glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap.ID);

	glActiveTexture(GL_TEXTURE0 + 11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, specFilteredMap.ID);

	glActiveTexture(GL_TEXTURE0 + 12);
	glBindTexture(GL_TEXTURE_2D, brdfLUT.ID);
}

void RenderContext::DrawSkybox()
{
	std::shared_ptr<Skybox> skybox = curScene->getSkybox();
	std::shared_ptr<Shader> shader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(SkyBoxshader);

	shader->Use();
	shader->setInt("environmentMap",0);
	skybox->draw();
}

void RenderContext::setupCameraProperties(std::shared_ptr<Camera> camera)
{
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useCamera) {
			shaderP.second->Use();
			shaderP.second->setMat4(CameraUniformNameList::view, camera->GetViewMatrix());
			shaderP.second->setMat4(CameraUniformNameList::projection, camera->GetProjectionMatrix());
			shaderP.second->setMat4(CameraUniformNameList::invView, camera->GetInvViewMatrix());
			shaderP.second->setMat4(CameraUniformNameList::invProjection, camera->GetInvProjectionMatrix());
			shaderP.second->setVec3(CameraUniformNameList::CamPos, camera->GetPosition());
			shaderP.second->setInt(CameraUniformNameList::Scrwidth,SCREEN_WIDTH);
			shaderP.second->setInt(CameraUniformNameList::Scrheight, SCREEN_HEIGHT);
		}
	}
}

void RenderContext::setupModelMatrix(std::shared_ptr<Model> model)
{
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useCamera) {
			shaderP.second->Use();
			shaderP.second->setMat4(CameraUniformNameList::model, model->getModelMatrix());
		}
	}
}

void RenderContext::setupLightProperties()
{
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useLight) {
			for (auto& light : curScene->getLights()) {
				if (light->type == "DirectionalLight") {
					shaderP.second->Use();
					shaderP.second->setVec3("LightColor",light->color);
					shaderP.second->setFloat("LightIntensity", light->intensity);
					shaderP.second->setVec3("LightDirection", std::dynamic_pointer_cast<DirectionalLight>(light)->direction);
				}

			}
		}
	}
}

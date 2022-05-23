#include "Passes\CSMshadowMap.h"
#include "Manager\SceneManager.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Light.h"
#include "OpenGLInterface\Camera.h"

void CSMshadowMap::init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	shadowMapShader = MaterialSystem::getOrCreateInstance()->registerShader("CSMshadowmapVs.glsl", "CSMshadowmapFs.glsl","CSMshadowmapGs.glsl");
	shadowArrayFBO.createRenderTarget(resolution, resolution, RenderTarget::ENUM_TYPE_DEPTH_ARRAY, Split);
	ShadowMatUBO = context->GenBuffer(GL_UNIFORM_BUFFER,sizeof(glm::mat4x4) * 16, NULL, GL_STATIC_DRAW,0);

	RenderManager::getOrCreateInstance()->registerPassData("DirDepthTex", shadowArrayFBO.getdepthTexture());
	RenderManager::getOrCreateInstance()->registerPassData("DistanceList", FarPlaneDistantList);
}

void CSMshadowMap::update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	mainLight = SceneManager::getOrCreateInstance()->getCurrentScene()->getMainLight();

	updateCameraFrustum(Rendercamera, mainLight->direction);
	
	shadowArrayFBO.use();
	RenderTarget::clear(GL_DEPTH_BUFFER_BIT);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, shadowArrayFBO.getdepthTexture().ID, 0); //这句好像没有什么意义

	context->DrawOpaqueRenderList(shadowMapShader, SceneManager::getOrCreateInstance()->getCurrentScene()->getModels());

	shadowArrayFBO.UseDefault();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void CSMshadowMap::updateCameraFrustum(std::shared_ptr<Camera>& Rendercamera, glm::vec3 lightDir)
{
	FarPlaneDistantList.clear();
	ClipPointList.clear();
	LightProViewMatList.clear();

	updateDistanceList(Rendercamera);
	updateClipPointByCamera(Rendercamera);
	updateLightViewMat(Rendercamera, lightDir);
	updateShadowMatUBO();
}

void CSMshadowMap::updateDistanceList(std::shared_ptr<Camera>& camera)
{
	//GPU GEMS3中的切分方法 不明觉厉
	for (unsigned i = 1; i < Split; i++)
	{
		float k = (float)i / (float)Split;
		//好像后面的层数有点太大了
		FarPlaneDistantList.push_back(0.8f * (camera->GetNearPlane() * powf(camera->GetFoV(), k)) + (1 - 0.8f) * (camera->GetNearPlane() + (camera->GetFarPlane() - camera->GetNearPlane()) * k));
	}
	FarPlaneDistantList.push_back(camera->GetFarPlane());
	RenderManager::getOrCreateInstance()->updatePassData("DistanceList", FarPlaneDistantList);

}

void CSMshadowMap::updateLightViewMat(std::shared_ptr<Camera>& Rendercamera, glm::vec3 lightDir)
{
	glm::mat4 lightViewMat = glm::lookAt(glm::vec3(0.f),lightDir, glm::vec3(0, 1, 0));

	for (unsigned i = 0; i < Split; i++)
	{
		glm::vec4 ClipPointVS = lightViewMat * ClipPointList[i][0];
		
		glm::vec3 tmax(-1000.0f, -1000.0f, -1000.0f);
		glm::vec3 tmin(1000.0f, 1000.0f, 1000.0f);

		for (unsigned int j = 0; j < 8; j++) {
			glm::vec4 pointVS = lightViewMat * glm::vec4(ClipPointList[i][j]);
			if (pointVS.z > tmax.z) { tmax.z = pointVS.z; }
			if (pointVS.z < tmin.z) { tmin.z = pointVS.z; }
			if (pointVS.x > tmax.x) { tmax.x = pointVS.x; }
			if (pointVS.x < tmin.x) { tmin.x = pointVS.x; }
			if (pointVS.y > tmax.y) { tmax.y = pointVS.y; }
			if (pointVS.y < tmin.y) { tmin.y = pointVS.y; }
		}
		tmax.z += 500.0f;

		glm::mat4 Ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -tmax.z, -tmin.z);

		glm::vec2 tscale(2.0f / (tmax.x - tmin.x), 2.0f / (tmax.y - tmin.y));
		glm::vec2 toffset(-0.5f * (tmax.x + tmin.x) * tscale.x, -0.5f * (tmax.y + tmin.y) * tscale.y);
		glm::mat4 shad_Crop = glm::mat4(1);

		shad_Crop[0][0] = tscale.x;
		shad_Crop[1][1] = tscale.y;
		shad_Crop[0][3] = toffset.x;
		shad_Crop[1][3] = toffset.y;
		shad_Crop = glm::transpose(shad_Crop);
        
		glm::mat4 ShadowCameraProjectionMat = shad_Crop * Ortho;
		LightProViewMatList.push_back(ShadowCameraProjectionMat * lightViewMat);
	}

}

void CSMshadowMap::updateClipPointByCamera(std::shared_ptr<Camera>& Rendercamera)
{
	glm::vec4 camPos = glm::vec4(Rendercamera->GetPosition(),1.0);
	glm::vec4 camDir = glm::vec4(Rendercamera->GetFront(),0.0);
	glm::vec4 camUp = glm::vec4(Rendercamera->GetUp(),0.0);
	glm::vec4 camRight = glm::vec4(Rendercamera->GetRight(),0.0);
	float aspect = SCREEN_ASPECT_RATIO;

	float curNearDistance = Rendercamera->GetNearPlane();
	float curFarDistance = FarPlaneDistantList[0];

	for(unsigned i = 0; i < FarPlaneDistantList.size(); i++) {
		if (i < FarPlaneDistantList.size()&&i>0)
		{
			curNearDistance = curFarDistance;
			curFarDistance = FarPlaneDistantList[i];
		}
		std::vector<glm::vec4> singleClipPointList;

		float nearHeight = tanf(Rendercamera->GetFoV() / 2.0f) * curNearDistance;
		float nearWidth = nearHeight * aspect;
		float farHeight = tanf(Rendercamera->GetFoV() / 2.0f) * curFarDistance;
		float farWidth = farHeight * aspect;

		glm::vec4 nearCenter = camPos + camDir * curNearDistance;
		glm::vec4 farCenter = camPos + camDir * curFarDistance;

		singleClipPointList.push_back(nearCenter - camUp * nearHeight - camRight * nearWidth);
		singleClipPointList.push_back(nearCenter - camUp * nearHeight + camRight * nearWidth);
		singleClipPointList.push_back(nearCenter + camUp * nearHeight - camRight * nearWidth);
		singleClipPointList.push_back(nearCenter + camUp * nearHeight + camRight * nearWidth);

		singleClipPointList.push_back(farCenter - camUp * farHeight - camRight * farWidth);
		singleClipPointList.push_back(farCenter + camUp * farHeight - camRight * farWidth);
		singleClipPointList.push_back(farCenter - camUp * farHeight + camRight * farWidth);
		singleClipPointList.push_back(farCenter + camUp * farHeight + camRight * farWidth);

		ClipPointList.push_back(singleClipPointList);
	}
   
}

void CSMshadowMap::updateShadowMatUBO()
{

	glBindBuffer(GL_UNIFORM_BUFFER, ShadowMatUBO);
	for (unsigned i = 0; i < LightProViewMatList.size(); ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4), sizeof(glm::mat4), &LightProViewMatList[i]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

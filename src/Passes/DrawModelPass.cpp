#include "OpenGLInterface\RenderContext.h"
#include "Manager\SceneManager.h"
#include "Manager\Scene.h"
#include "Passes\DrawModelPass.h"

void DrawModelPass::init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
}

void DrawModelPass::update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera)
{
	RenderTarget::clear();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	context->setupCameraProperties(Rendercamera);

	context->DrawOpaqueRenderList(SceneManager::getOrCreateInstance()->getCurrentScene()->getModels());
}

#include "OpenGLInterface\RenderContext.h"
#include "Manager\MaterialSystem.h"
#include "Manager\SceneManager.h"
#include "OpenGLInterface\Model.h"
#include "OpenGLInterface\Shader.h"
#include "OpenGLInterface\Camera.h"

void RenderContext::DrawOpaqueRenderList(std::vector<std::shared_ptr<Model>>& opaquemodels)
{
	for (auto& model : opaquemodels) {
		setupModelMatrix(model);
		model->Draw();
	}
}

void RenderContext::DrawOpaqueRenderList(Shaderid shaderid, std::vector<std::shared_ptr<Model>>& opaquemodels)
{
	std::shared_ptr<Shader> shader = MaterialSystem::getOrCreateInstance()->getRegisterShaderByID(shaderid);
	shader->Use();	
	for (auto& model : opaquemodels) {
		shader->setMat4(CameraUniformNameList::model, model->getModelMatrix());
		model->DefaultDraw();
	}
}

void RenderContext::setupCameraProperties(std::shared_ptr<Camera> camera)
{
	for (auto& shaderP : MaterialSystem::getOrCreateInstance()->getRegisterShaderList())
	{
		if (shaderP.second->useCamera) {
			shaderP.second->Use();
			shaderP.second->setMat4(CameraUniformNameList::view, camera->GetViewMatrix());
			shaderP.second->setMat4(CameraUniformNameList::projection, camera->GetProjectionMatrix());
			shaderP.second->setVec3(CameraUniformNameList::CamPos, camera->GetPosition());
		}
	}
}

unsigned RenderContext::GenBuffer(GLenum Target, GLsizeiptr size, GLvoid* Data, GLenum usage, GLint BindingIndex)
{
	unsigned BufferID;
	glGenBuffers(1, &BufferID);
	glBindBuffer(Target, BufferID);
	glBufferData(Target, size, Data, usage);
	glBindBuffer(Target, 0);
	if (BindingIndex != -1)
		glBindBufferBase(Target, BindingIndex, BufferID);
	return BufferID;
}

void RenderContext::UpdateDatainBuffer(GLenum Target, GLint BufferID, GLintptr offset, GLsizeiptr size, const GLvoid* Data)
{
	glBindBuffer(Target, BufferID);
	glBufferSubData(Target, offset, size, Data);
	glBindBuffer(Target, 0);
}

void RenderContext::UpdateDatainBuffer(GLenum Target, GLint BufferID, std::vector<GLintptr> offsets, std::vector<GLsizeiptr> sizes, std::vector<const GLvoid*> Datas)
{
	if (offsets.size() != Datas.size() || sizes.size() != Datas.size()) return;
	glBindBuffer(Target, BufferID);
	for (auto i = 0; i < Datas.size(); ++i)
	{
		glBufferSubData(Target, offsets[i], sizes[i], Datas[i]);
	}
	glBindBuffer(Target, 0);
}

void RenderContext::ChangeDatainBuffer(GLenum Target, GLuint BufferID, GLsizeiptr size, GLvoid* Data, GLenum usage)
{
	glBindBuffer(Target, BufferID);
	glBufferData(Target, size, Data, usage);
	glBindBuffer(Target, 0);
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


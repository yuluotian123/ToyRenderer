#pragma once
#include "OpenGLInterface\RenderTarget.h"
#include <memory>
#include <vector>
#include "Base\common.h"

class Camera;
class Model;
namespace CameraUniformNameList {//（为什么c++没有静态类？）并不一定必要 因为每帧调用一次的话 消耗其实不高
	const std::string view = "ViewMatrix";
	const std::string projection = "ProjectionMatrix";
	const std::string CamPos = "CameraPos";
	const std::string NearPlane = "NearPlane";
	const std::string FarPlane = "FarPlane";
};

class RenderContext
{
public:
	void DrawOpaqueRenderList(std::vector<std::shared_ptr<Model>>& opaquemodels);
	void DrawOpaqueRenderList(Shaderid shaderid, std::vector<std::shared_ptr<Model>>& opaquemodels);

	void setupCameraProperties(std::shared_ptr<Camera> camera);

	//UBOandFBO maybe      VBO（not use for now）
	unsigned GenBuffer(GLenum Target,GLsizeiptr size,GLvoid* Data,GLenum usage,GLint BindingIndex);
	//需要重新设计 现在完全用不着
	void UpdateDatainBuffer(GLenum Target, GLint BufferID, GLintptr offset, GLsizeiptr size, const GLvoid* Data );
	void UpdateDatainBuffer(GLenum Target, GLint BufferID, std::vector<GLintptr> offsets, std::vector <GLsizeiptr> sizes, std::vector<const GLvoid*> Datas);
	void ChangeDatainBuffer(GLenum Target, GLuint BufferID, GLsizeiptr size, GLvoid* Data,GLenum usage);

	void GetDatainBuffer(GLenum Target,GLint BufferID, GLsizeiptr size, GLvoid** Data);
};
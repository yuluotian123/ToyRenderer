#pragma once
#include "OpenGLInterface\RenderTarget.h"
#include <memory>
#include <vector>
#include "Base\common.h"

class Camera;
class Model;
namespace CameraUniformNameList {//��Ϊʲôc++û�о�̬�ࣿ��
	const std::string view = "ViewMatrix";
	const std::string projection = "ProjectionMatrix";
	const std::string CamPos = "CameraPos";
	const std::string model = "ModelMatrix";
};

//ΪʲôҪ��rendermanager���ﴫ����������أ� ��Ϊ�Ժ���ܻ���һ�������������
class RenderContext
{
public:
	void DrawOpaqueRenderList(std::vector<std::shared_ptr<Model>>& opaquemodels);
	void DrawOpaqueRenderList(Shaderid shaderid, std::vector<std::shared_ptr<Model>>& opaquemodels);

	void setupCameraProperties(std::shared_ptr<Camera> camera);

	//UBOandFBO maybe VBO��not use for now��
	unsigned GenBuffer(GLenum Target,GLsizeiptr size,GLvoid* Data,GLenum usage,GLint BindingIndex);
	void UpdateDatainBuffer(GLenum Target, GLint BufferID, GLintptr offset, GLsizeiptr size, const GLvoid* Data );
	void UpdateDatainBuffer(GLenum Target, GLint BufferID, std::vector<GLintptr> offsets, std::vector <GLsizeiptr> sizes, std::vector<const GLvoid*> Datas);
	void ChangeDatainBuffer(GLenum Target, GLuint BufferID, GLsizeiptr size, GLvoid* Data,GLenum usage);
private:
	void setupModelMatrix(std::shared_ptr<Model> model);//���ǽ��˷�������Model->Draw
};
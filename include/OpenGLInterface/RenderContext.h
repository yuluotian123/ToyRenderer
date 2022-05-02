#pragma once
#include <memory>

class Camera;
class Model;
class RenderContext
{
public:
	void setupCameraProperties(std::shared_ptr<Camera> camera);

	void setupModelMatrix(std::shared_ptr<Model> model);//���ǽ��˷�������Model->Draw
};


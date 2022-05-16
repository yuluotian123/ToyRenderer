#pragma once
#include "BasePass.h"
class DrawModelPass :
    public BasePass
{
public:
	DrawModelPass(const std::string& PassName, int Order)
		:BasePass(PassName, Order) {};

	void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
	void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;

private:
	void setClusterProperties(std::shared_ptr<Camera>& camera);
	void setCSMProperties();
};


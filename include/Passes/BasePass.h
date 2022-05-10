#pragma once
#include "Base\Common.h"
#include <memory>
enum class RenderPassType:int
{
	PassType_Normal,
	PassType_Once,
	PassType_Parallel,
	PassType_ParallelOnce,
	PassType_Delay,
};

class RenderContext;
class Camera;
class BasePass
{
public:
	BasePass(const std::string& _PassName, int _Order)
		:PassName(_PassName),Order(_Order),PassType(RenderPassType::PassType_Normal){};
	BasePass(const std::string& _PassName, int _Order, RenderPassType type)
		:PassName(_PassName), Order(_Order), PassType(type){};
	virtual ~BasePass() {};

	virtual void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) = 0;
	virtual void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) = 0;

	const std::string& getPassName() { return PassName; };
	const int getOrder() { return Order; };
	RenderPassType getRendertype() { return PassType; };

	void setPassName(const std::string& _PassName) { PassName = _PassName; };
	void setOrder(int _Order) { Order = _Order; };
	void setPassType(RenderPassType Type) { PassType = Type; };

	void finish() { Order = -1; };
private:
	std::string PassName;
	int Order;
	RenderPassType PassType;
};


#pragma once
#include "Passes\BasePass.h"
//用于clusterlight里的ssbo 我在想如果以后都用clusterlight那是不是可以不需要PointLight(内存对齐很重要！)
struct GPUpointLight {
	glm::vec4 position;
	glm::vec4 color;
	float radius;
	float intensity;
	float padding[2];
};

struct clusterAABB {
	glm::vec4 minPoint;
	glm::vec4 maxPoint;
};

constexpr unsigned int maxLightCount = 80;
constexpr unsigned int gridSizeX = 24*SCREEN_ASPECT_RATIO;
constexpr unsigned int gridSizeY = 24;
constexpr unsigned int gridSizeZ = 30;
constexpr unsigned int clusterNum = gridSizeX * gridSizeY * gridSizeZ;
constexpr unsigned int maxLightPerCluster = 25;

class Light;
class ClusterLight:public BasePass
{
public:
	ClusterLight(const std::string& PassName, int Order) 
		:BasePass(PassName,Order){};

	void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;
	void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;

	void updateLightSSBO(std::vector<std::shared_ptr<Light>>& lights);
	void Cluster_AABBPass(std::shared_ptr<Camera>& Rendercamera);
	void LightCullingPass(std::shared_ptr<Camera>& Rendercamera);
	void setMainLightProperties(std::vector<std::shared_ptr<Light>>& lights);

	static bool Update;
private:
	Shaderid ClusterShader,LightCullingShader;

	unsigned LightSSBO,aabbSSBO,LightListSSBO, LightGridSSBO;
	unsigned int PointLightCount = 0;
};


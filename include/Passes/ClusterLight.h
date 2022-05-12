#pragma once
#include "Passes\BasePass.h"
//用于clusterlight里的ssbo 我在想如果以后都用clusterlight那是不是可以不需要PointLight
struct GPUpointLight {
	float radius;
	glm::vec3 position;
	glm::vec3 Color;
	float Intensity;
};

struct clusterAABB {
	glm::vec4 minPoint;
	glm::vec4 maxPoint;
};

constexpr unsigned int maxLightCount = 80;
constexpr unsigned int gridSizeX = 10*SCREEN_ASPECT_RATIO;
constexpr unsigned int gridSizeY = 10;
constexpr unsigned int gridSizeZ = 24;
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
private:
	Shaderid ClusterShader,LightCullingShader;

	unsigned LightSSBO,aabbSSBO,LightListSSBO, LightGridSSBO;
	unsigned int PointLightCount = 0;
};


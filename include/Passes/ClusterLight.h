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
private:
	Shaderid ClusterShader = -1;

	unsigned LightSSBO = -1;
	unsigned aabbSSBO = -1;
	unsigned LightListSSBO = -1;

	unsigned int PointLightCount = 0;
	const unsigned int maxLightCount = 80;
	const unsigned int gridSizeX = 16;
	const unsigned int gridSizeY = 9;
	const unsigned int gridSizeZ = 24;
	const unsigned int clusterNum = gridSizeX * gridSizeY * gridSizeZ;
	const unsigned int maxLightPerCluster = 24;
};


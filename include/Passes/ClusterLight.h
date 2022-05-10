#pragma once
#include "Passes\BasePass.h"
//����clusterlight���ssbo ����������Ժ���clusterlight���ǲ��ǿ��Բ���ҪPointLight
struct GPUpointLight {
	float radius;
	glm::vec3 position;
	glm::vec3 Color;
	float Intensity;
};

class ClusterLight:public BasePass
{
public:
	ClusterLight(const std::string& PassName, int Order) 
		:BasePass(PassName,Order){};

	void init(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;

	void update(std::shared_ptr<RenderContext>& context, std::shared_ptr<Camera>& Rendercamera) override;

	void updateLightSSBO(std::vector<std::shared_ptr<Light>>& lights);//������ssbo������һ�������� �ȽϺÿ�
	void Cluster_AABBPass(std::shared_ptr<Camera>& Rendercamera);
public:
	Shaderid ClusterShader = -1;

	unsigned LightSSBO = -1;
	unsigned int PointLightCount = 0;
	const unsigned int maxLightCount = 80;
	const unsigned int gridSizeX = 16;
	const unsigned int gridSizeY = 9;
	const unsigned int gridSizeZ = 24;
};


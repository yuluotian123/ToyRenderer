#include "Base\AABB.h"
   
AABB::AABB(const glm::vec3& _min, const glm::vec3& _max)
{
	min = _min;
	max = _max;
}

AABB::AABB(const std::vector<glm::vec3>& points)
{
	min = { FLT_MAX,FLT_MAX ,FLT_MAX };
	max = { -FLT_MAX,-FLT_MAX ,-FLT_MAX };

	for (unsigned i = 0; i < points.size(); i++)
		combine(points[i]);
}

void AABB::combine(const glm::vec3& point)
{
	min = glm::min(min, point);
	max = glm::max(max, point);
}

void AABB::combine(const AABB& aabb)
{
	min = glm::min(min, aabb.min);
	max = glm::max(max, aabb.max);
}

float AABB::getVolume()
{
	return (max.x-min.x)* (max.y - min.y)* (max.z - min.z);
}
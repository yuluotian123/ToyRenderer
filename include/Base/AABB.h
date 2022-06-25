#pragma once
#include "glm\glm.hpp"
#include <vector>
class AABB
{
public:
	AABB()=default;
	AABB(const glm::vec3 & _min, const glm::vec3 & _max);
	AABB(const std::vector<glm::vec3>& points);
	~AABB() {};

	void combine(const glm::vec3& point);
	void combine(const AABB& AABB);
	bool inside(const glm::vec3& pos);

	float getVolume();
	const glm::vec3 getMin() const { return min; };
	const glm::vec3 getMax()  const { return max; };
private:
	glm::vec3 min;
	glm::vec3 max;
};


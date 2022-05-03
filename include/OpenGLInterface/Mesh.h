#pragma once
#include "OpenGLInterface\Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <memory>

class Material;
struct Vertex//顶点数据Attribs
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::vec2 texCoords;
};

struct MaterialProperties//材质中通过loadModel获得的数据(疑似没啥用而且很耗费性能)
{
	glm::vec3 AmbientColor = {0.f,0.f,0.f};
	glm::vec3 DiffuseColor = { 0.f,0.f,0.f };
	glm::vec3 SpecularColor = { 0.f,0.f,0.f };
	float Shininess = 0.0f;
	float Refracti = 0.0f;
};
class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const MaterialProperties& matproperties)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->matproperties = matproperties;
		setupMesh();
	}

	void setupMesh();//设置attribs

	void setMaterial(const Materialid& Type);//设置每个Mesh的material
	void Draw() const;//更新uniform以及绘制mesh(采用material内置的参数)
	void Draw(Shaderid shaderid) const; //用shader来绘制mesh

	std::shared_ptr<Material>& getMaterial() { return material; };//获取当前的material
private:
	unsigned int VAO,EBO,VBO;

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	MaterialProperties matproperties;
	std::shared_ptr<Material> material;
};


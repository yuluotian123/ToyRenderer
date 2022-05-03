#pragma once
#include "OpenGLInterface\Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <memory>

class Material;
struct Vertex//��������Attribs
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::vec2 texCoords;
};

struct MaterialProperties//������ͨ��loadModel��õ�����(����ûɶ�ö��Һܺķ�����)
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

	void setupMesh();//����attribs

	void setMaterial(const Materialid& Type);//����ÿ��Mesh��material
	void Draw() const;//����uniform�Լ�����mesh(����material���õĲ���)
	void Draw(Shaderid shaderid) const; //��shader������mesh

	std::shared_ptr<Material>& getMaterial() { return material; };//��ȡ��ǰ��material
private:
	unsigned int VAO,EBO,VBO;

	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	MaterialProperties matproperties;
	std::shared_ptr<Material> material;
};

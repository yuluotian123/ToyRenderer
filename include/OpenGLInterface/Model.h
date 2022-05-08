#pragma once
#include "OpenGLInterface\Mesh.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <memory>
#include <vector>
#include <unordered_set>

//���ǽ�transform�½�Ϊһ����������
struct Transform {
    glm::vec3 translation;
    float angle;
    glm::vec3 rotationAxis;
    glm::vec3 scaling;
    Transform() {
        translation = { 0.f,0.f,0.f };
        angle = 0.f;
        rotationAxis = { 1.f,1.f,1.f };
        scaling = { 1.f,1.f,1.f };
    }
    Transform(const Transform& tras) {
        translation = tras.translation;
        angle = tras.angle;
        scaling = tras.scaling;
        rotationAxis = tras.rotationAxis;
    }
};

//Ŀǰ�������嶼��modelΪ��λ 
class Model
{
public:
    Model(const std::string& meshPath, const Transform initTransform)
        :trans(initTransform) {
        loadModel(meshPath);
    }
    Model(const std::string& meshPath) {
        loadModel(meshPath);
    }

    void Draw() const;//ģ�͵���Ⱦ������mesh��draw����
    void DefaultDraw() const;//��shader������ģ����Ⱦ(���ٵ���material)
    void Update(float DeltaTime); //����ģ�͵��ƶ��ȶ���

    void SetMaterials(const Materialid& Materialid);//һ��������Model������mesh��material

    std::vector<std::shared_ptr<Mesh>>& getMeshes() { return meshes; };
    std::shared_ptr<Mesh> getMeshbyIndex(int Index);

    //��UI��
    glm::vec3* getPositionp() { return &trans.translation; };
    glm::vec3* getScallingp() { return &trans.scaling; };
    glm::vec3* getRotationAxisp() { return &trans.rotationAxis; };
    float* getAnglep() { return &trans.angle; };

    const glm::mat4& getModelMatrix() { UpdateModelMatrix();  return modelMatrix; };//����ô�����ĺ��𣨸о�ÿ�θ��¶�update�е��˷ѣ�
    void UpdateModelMatrix();

    const std::string& getDirectory() { return directory; };
private:
    //��ȡmesh�Լ�texture��MatProperties���ݣ�matProperties������ʱû��ʹ�ã���Ϊϣ�����ڿ������û��Լ����ã�
    void loadModel(const std::string& meshPath);

    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    void processMatProperties(const aiMesh* mesh, MaterialProperties& matproperties);
    void processVertex(const aiMesh* mesh, std::vector<Vertex>& vertices);
    void processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices);
    void processTextures(const aiMesh* mesh, std::vector<Texture>& textures);
    void loadTextures(aiMaterial* material, std::vector<Texture>& textures, aiTextureType type, const std::string& typeName);
private:
    const aiScene* m_Scene;

    std::string directory;
    std::vector<Texture> load_textures;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::unordered_set<Materialid> materialList;//���ڲ��ҵ�ǰmodel�õ�������material������û�����ô��ƣ�

    glm::mat4 modelMatrix;
    Transform trans;
};


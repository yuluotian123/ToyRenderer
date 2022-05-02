#pragma once
#include "OpenGLInterface\Mesh.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <memory>
#include <vector>

//考虑将transform新建为一个单独的类
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

//目前所有物体都以model为单位 
class Model
{
public:
    Model(const std::string meshPath, const Transform initTransform)
        :trans(initTransform){
        loadModel(meshPath);
    }
    Model(const std::string meshPath) {
        loadModel(meshPath);
    }

    void Draw() const;//模型的渲染，调用mesh的draw方法
    void Update(float DeltaTime); //处理模型的移动等动画

    void SetMaterials(const Materialid& Materialid);//一次性设置Model中所有mesh的material

    std::vector<std::shared_ptr<Mesh>>& getMeshes(){return meshes; };
    std::shared_ptr<Mesh> getMeshbyIndex(int Index);

    //给UI用
    glm::vec3* getPositionp() { return &trans.translation; };
    glm::vec3* getScallingp() { return &trans.scaling; };
    glm::vec3* getRotationAxisp() { return &trans.rotationAxis; };
    float* getAnglep() { return &trans.angle; };

    const glm::mat4& getModelMatrix() { UpdateModelMatrix();  return modelMatrix; };//？这么设计真的好吗（感觉每次更新都update有点浪费）
    void UpdateModelMatrix();

    const std::string& getDirectory() { return directory; };
private:
    //获取mesh以及texture，MatProperties数据（matProperties数据暂时没有使用，因为希望后期可以让用户自己设置）
    void loadModel(std::string meshPath);

    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    void processMatProperties(const aiMesh* mesh, const aiScene* scene, MaterialProperties& matproperties);
    void processVertex(const aiMesh* mesh, std::vector<Vertex>& vertices);
    void processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices);
    void processTextures(const aiMesh* mesh, const aiScene* scene, std::vector<Texture>& textures);
    void loadTextures(aiMaterial* material, std::vector<Texture>& textures, aiTextureType type, std::string typeName);
private:
    std::string directory;
    std::vector<Texture> load_textures;
    std::vector<std::shared_ptr<Mesh>> meshes;

    glm::mat4 modelMatrix;
    Transform trans;
};


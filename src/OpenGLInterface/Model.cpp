#include "OpenGLInterface\Material.h"
#include "OpenGLInterface\Model.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Shader.h"
#include <iostream>

void Model::Draw() const
{
    if (meshes.empty()) return;
    for (auto& mesh : meshes)
        mesh->Draw();

}

void Model::DefaultDraw() const
{
    if (meshes.empty()) return;
    for (auto& mesh : meshes)
        mesh->DefaultDraw();
}

void Model::Update(float DeltaTime)
{
}

void Model::SetMaterials(const Materialid& Materialid)
{
    if (meshes.empty()) {
        printf("There is no mesh in this model:%s.\n",directory.c_str());
        return;
    }
    for (auto& mesh : meshes) {
        mesh->setMaterial(Materialid);
    }
}

std::shared_ptr<Mesh> Model::getMeshbyIndex(int Index)
{
    if(Index>meshes.size()) { printf("can't find mesh at index:%d.return null.\n", Index);  return nullptr; }
    return meshes[Index];
}

void Model::UpdateModelMatrix()
{
    modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::translate(modelMatrix, trans.translation);
    modelMatrix = glm::rotate(modelMatrix, trans.angle, trans.rotationAxis);
    modelMatrix = glm::scale(modelMatrix, trans.scaling);
}

//仿照learningOpenGL实现
void Model::loadModel(const std::string& path)
{
	Assimp::Importer ModelImpoter;
    m_Scene = ModelImpoter.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (!m_Scene || !m_Scene->mRootNode || m_Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "error: " << ModelImpoter.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(m_Scene->mRootNode, m_Scene);
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
	//Process all the node meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//process all the node children recursively
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Mesh>  Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    MaterialProperties matproperties;

    processVertex(mesh, vertices);
    processIndices(mesh, indices);
    processMatProperties(mesh,  matproperties);
    processTextures(mesh, textures);

    return std::make_shared<Mesh>(vertices,indices,textures,matproperties);
}

void Model::processMatProperties(const aiMesh* mesh, MaterialProperties& matproperties)
{
    if (mesh->mMaterialIndex < 0)
        return;
    aiMaterial* mat = m_Scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D AmbientColor, DiffuseColor, SpecularColor;
    float Shininess = 0.0f, Refracti = 0.0f;
    mat->Get(AI_MATKEY_COLOR_AMBIENT, AmbientColor);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor);
    mat->Get(AI_MATKEY_SHININESS, Shininess);
    mat->Get(AI_MATKEY_REFRACTI, Refracti);
    matproperties.AmbientColor = { AmbientColor.r, AmbientColor.g, AmbientColor.b };
    matproperties.DiffuseColor = { DiffuseColor.r, DiffuseColor.g, DiffuseColor.b };
    matproperties.SpecularColor = { SpecularColor.r, SpecularColor.g, SpecularColor.b };
    matproperties.Shininess = Shininess;
    matproperties.Refracti = Refracti;
}

void Model::processVertex(const aiMesh* mesh, std::vector<Vertex>& vertices)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        //Process vertex positions, normals, tangents, bitangents, and texture coordinates
        Vertex vertex;
        glm::vec3 vector;

        //Process position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        //Process tangent
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tangent = vector;

        //Process biTangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.biTangent = vector;

        //Process normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        //Process texture coords
        if (mesh->HasTextureCoords(0)) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
}

void Model::processIndices(const aiMesh* mesh, std::vector<unsigned int>& indices)
{
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void Model::processTextures(const aiMesh* mesh, std::vector<Texture>& Textures)
{
    if (mesh->mMaterialIndex < 0)
        return;
    aiMaterial* mat = m_Scene->mMaterials[mesh->mMaterialIndex];

    //目前只接受这些
    loadTextures(mat,Textures,aiTextureType_DIFFUSE,"DiffuseMap");
    loadTextures(mat, Textures, aiTextureType_AMBIENT, "AmbientMap");
    loadTextures(mat,Textures,aiTextureType_SPECULAR,"SpecularMap");
    loadTextures(mat, Textures, aiTextureType_NORMALS, "NormalMap");
    loadTextures(mat, Textures, aiTextureType_HEIGHT,"HeightMap");
    loadTextures(mat, Textures, aiTextureType_SHININESS,"ShinessMap");
    loadTextures(mat, Textures, aiTextureType_EMISSIVE,"EmissiveMap");
    loadTextures(mat, Textures, aiTextureType_UNKNOWN, "MetalRoughMap");
}

void Model::loadTextures(aiMaterial* mat, std::vector<Texture>& textures, aiTextureType type, const std::string& typeName)
{
    int TextureCount = mat->GetTextureCount(type);
    int TextureIndex = -1;
    if (TextureCount <= 0) return;
    for (unsigned int i = 0; i < TextureCount; ++i)
    {
        aiString Str;
        mat->GetTexture(type, i, &Str);
        std::string TextureName = Str.C_Str();
        std::string TexturePath = directory + "/" + TextureName;
        std::replace(TexturePath.begin(), TexturePath.end(), '\\', '/');
        bool Skip = false;
        int LoadedTextureCount = static_cast<int>(load_textures.size());
        for (int k = 0; k < LoadedTextureCount; ++k)
        {
            if (TexturePath == load_textures[k].path)
            {
                Skip = true;
                load_textures[k].typeName = typeName+ std::to_string(++TextureIndex);
                textures.push_back(load_textures[k]);
                break;
            }
        }
        if (!Skip)
        {
            Texture tex;
            tex.loadTexture(TexturePath, false);
            tex.typeName = typeName + std::to_string(++TextureIndex);
            textures.push_back(tex);
            load_textures.push_back(tex);
        }
    }
}
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Mesh.h"
#include "Manager\MaterialSystem.h"
#include "OpenGLInterface\Material.h"

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //VBO 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //Position pointer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //Normal pointer
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    //Vertex texturecoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    //Tangent position
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    //Bittangent position
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));

    //Unbinding VAO
    glBindVertexArray(0);
}

void Mesh::setMaterial(const  Materialid& Type)//创建新材质 （以及替换旧的材质）
{
    if(material)
    {
        auto& map = MaterialSystem::getOrCreateInstance()->getRegisterMaterialList();

        for (auto it = map[material->getMaterialType()].begin(); it != map[material->getMaterialType()].end(); it++) {
            if (*it == material)
            {
                map[material->getMaterialType()].erase(it);
                break;
            }
        }
    }

    material = MaterialSystem::getOrCreateInstance()->CreateMaterial(Type);

    if (!material) {
        printf("fail to get material.\n");
    }

    material->RegisterMeshData(textures);
}

void Mesh::Draw() const
{
    if (!material){
        printf("please attach a material on the mesh.");
    }

    material->UpdateUniform();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "OpenGLInterface\Primitive.h"

void Primitive::draw(const unsigned int readTex1, const unsigned int readTex2, const unsigned int readTex3)
{
    glBindVertexArray(VAO);

    //This texture read could be compacted into a for loop and an array could be passed instead
    //But for now this is sufficient 
    if (readTex1 != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, readTex1);
    }

    //A texture id of 0 is never assigned by opengl so we can
    //be sure that it means we haven't set any texture in the second paramenter and therefore
    //we only want one texture
    if (readTex2 != 0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, readTex2);
    }

    if (readTex3 != 0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, readTex3);
    }

    glDrawArrays(GL_TRIANGLES, 0, verticesNum);

}

void Cube::setupMesh()
{
    const float boxVertices[108] = {
       -1.0f, 1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,

       -1.0f, -1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

       -1.0f, -1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, -1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,

       -1.0f, 1.0f, -1.0f,
       1.0f, 1.0f, -1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, -1.0f,

       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, 1.0f };

    //Generate Buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind Vertex Array Object and VBO in correct order
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //VBO initialization 
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), &boxVertices, GL_STATIC_DRAW);

    //Vertex position pointer init
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //Unbinding VAO
    glBindVertexArray(0);
}

void Quad::setupMesh()
{
    const float quadVertices[] = {
        //positions //texCoordinates
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    //OpenGL postprocessing quad setup
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind Vertex Array Object and VBO in correct order
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //VBO initialization
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    //Quad position pointer initialization in attribute array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //Quad texcoords pointer initialization in attribute array
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

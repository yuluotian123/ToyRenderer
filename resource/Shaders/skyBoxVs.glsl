#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 localPos;

void main()
{
    localPos = aPos;

    mat4 rotView = mat4(mat3(ViewMatrix)); // remove translation from the view matrix
    vec4 clipPos = ProjectionMatrix *  rotView  * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}
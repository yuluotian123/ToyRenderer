#version 430 core
layout (location = 0 )in vec3 aPos;

uniform mat4 captureProjection;
uniform mat4 captureView;

out vec3 localPos;

void main()
{
    localPos = aPos;
    gl_Position = captureProjection * captureView * vec4(localPos, 1.0);
}
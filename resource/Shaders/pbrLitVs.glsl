#version 430 core
layout (location = 0 )in vec3 aPos;
layout (location = 2)in vec2 aTexCoords;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 Texcoord;

void main()
{
     Texcoord =  aTexCoords;
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPos, 1.0f);
}
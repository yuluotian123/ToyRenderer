#version 430 core
uniform sampler2D DiffuseMap0;
uniform vec3 DiffuseColor;

in vec2 Texcoord;

out vec4 FragColor;

void main()
{
	 FragColor = texture(DiffuseMap0,Texcoord);
}
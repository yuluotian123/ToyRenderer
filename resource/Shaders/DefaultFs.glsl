#version 430 core

in V_Out{
in  vec2 TexCoord;
in  vec3 WorldPos;
in  vec3 Normal;
in  mat3 TBN;
} f_in;

out vec4 FragColor;

void main(){
  FragColor = vec4(1.f);
}
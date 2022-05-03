#version 430 core
layout (location = 0 )in vec3 aPos;
layout (location = 1 )in vec3 aNormals;
layout (location = 2 )in vec2 aTexCoords;
layout (location = 3 )in vec3 aTangents;
layout (location = 4 )in vec3 aBiTangents;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out V_Out{
out vec2 TexCoord;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;
} v_out;

void main()
{
    v_out.WorldPos = vec3(ModelMatrix * vec4(aPos, 1.0f));
    v_out.TexCoord =  aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    v_out.Normal = normalMatrix * aNormals;

    vec3 T = normalize(normalMatrix * aTangents);
    vec3 B = normalize(normalMatrix * aBiTangents);
    vec3 N = normalize(normalMatrix * aNormals);    
    //切线空间转化（好像有的模型不能适用）
    v_out.TBN = transpose(mat3(T, B, N));  

	gl_Position = ProjectionMatrix * ViewMatrix *vec4(v_out.WorldPos,1.0f);
}
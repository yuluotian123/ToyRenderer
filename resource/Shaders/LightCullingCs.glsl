#version 430 core

layout(local_size_x = 1, local_size_y = 1) in;

struct voAABB{
    vec4 minPoint;
	vec4 maxPoint;
};

layout(std430,binding = 1) buffer clusterAABB{
  voAABB cluster[];
};

struct PointLight{
    float radius;
	vec3 position;
	vec3 Color;
	float Intensity;
};

layout(std430,binding = 2) buffer PointLights{
  PointLight light[];
};

layout(std430,binding = 3) buffer LightList{
  uint LightIndexList[];
};

struct LightGrid{
  uint count;
  uint start;
};

layout(std430,binding = 4) buffer LightList{
  LightGrid LightGridList[];
};

uniform mat4 ViewMatrix;
uniform int c;

bool PointSphereIntersect(float3 p, float4 shpere)
{
    
}

bool sphereIntersect( voAABB box,PointLight light){
  vec4 shpere = vec4(light.position, light.radius);


}

void main(){

    uint lightCount = light.length();
    uint tileIndex = gl_WorkGroupID.x +
                     gl_WorkGroupID.y * gl_NumWorkGroups.x +
                     gl_WorkGroupID.z * (gl_NumWorkGroups.x * gl_NumWorkGroups.y);
    
    voAABB box = cluster[tileIndex];

    uint startIndex = tileIndex * MaxLightPerTile;
    uint endIndex = startIndex;

    for(uint i = 0;i<light.length();i++){
    PointLight p = light[i];
    if(!sphereIntersect(box,p)) ccontinue;
    LightIndexList[endIndex++] = i;
    }

    LightGrid idx;
    idx.count = endIndex-startIndex;
    idx.start
}
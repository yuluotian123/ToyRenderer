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
	vec3 color;
	float intensity;
};

layout(std430,binding = 2) buffer PointLights{
  PointLight light[];
};

layout(std430,binding = 3) buffer LightList{
  uint LightIndexList[];
};

struct LightGrid{
  uint count;
  uint offset;
};

layout(std430,binding = 4) buffer LightGridList{
  LightGrid Lightgrid[];
};

uniform mat4 ViewMatrix;
uniform int MaxLightPerTile;

float PointSphereDistance(vec3 point, voAABB box){
   float Dis = 0;

    for(int i = 0; i < 3; ++i){
        float v = point[i];
        if(v < box.minPoint[i]){
            Dis += (box.minPoint[i] - v) * (box.minPoint[i] - v);
        }
        if(v > box.maxPoint[i]){
            Dis += (v - box.maxPoint[i]) * (v - box.maxPoint[i]);
        }
    }

    return Dis;

}

//¼òµ¥Çó½»
bool sphereIntersect( voAABB box,PointLight light)
{
   float radius = light.radius;
   vec3 position = vec3(ViewMatrix * vec4(light.position,1.0f));
   float Distance = PointSphereDistance(position,box);

   return Distance <= (radius*radius);
}

void main(){

    uint lightCount = light.length();
    uint tileIndex = gl_WorkGroupID.x +
                     gl_WorkGroupID.y * gl_NumWorkGroups.x +
                     gl_WorkGroupID.z * (gl_NumWorkGroups.x * gl_NumWorkGroups.y);
    
    voAABB box = cluster[tileIndex];
    box.maxPoint.w = tileIndex;

    uint startIndex = tileIndex * MaxLightPerTile;
    uint endIndex = startIndex;

    for(uint i = 0;i<light.length();i++){
    PointLight p = light[i];
    if(sphereIntersect(box,p)) LightIndexList[endIndex++] = i;
    }

    LightGrid idx;
    idx.count = endIndex-startIndex;
    idx.offset = startIndex;

    Lightgrid[tileIndex] = idx;
}
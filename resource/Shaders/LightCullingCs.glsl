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
	vec4 position;
	vec4 color;
    float radius;
	float intensity;
    float padding[2];
};

layout(std430,binding = 2) buffer PointLights{
  PointLight plight[];
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

uniform int PointLightLength;
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
   vec3 position = vec3(ViewMatrix * light.position);
   float Distance = PointSphereDistance(position,box);

   return Distance <= (radius * radius);
}

void main(){    
    uint tileIndex = gl_WorkGroupID.x +
                     gl_WorkGroupID.y * gl_NumWorkGroups.x +
                     gl_WorkGroupID.z * (gl_NumWorkGroups.x * gl_NumWorkGroups.y);
    
    voAABB box = cluster[tileIndex];
    box.maxPoint.w = tileIndex;

    uint startIndex = tileIndex * MaxLightPerTile;
    uint endIndex = startIndex;

    for(int i = 0;i< PointLightLength;i++){
      PointLight p = plight[i];
      if(sphereIntersect(box,p)) LightIndexList[endIndex++] = i;
    }
    barrier();

    LightGrid idx;
    idx.count = endIndex - startIndex;
    idx.offset = startIndex;
      barrier();

    Lightgrid[tileIndex] = idx;
}
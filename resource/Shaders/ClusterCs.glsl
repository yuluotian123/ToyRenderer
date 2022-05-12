#version 430 core

layout(local_size_x = 1, local_size_y = 1) in;

struct voAABB{
    vec4 minPoint;
	vec4 maxPoint;
};

layout(std430,binding = 1) buffer clusterAABB{
  voAABB cluster[];
};

uniform float NearPlane;
uniform float FarPlane;
uniform int ScreenWidth;
uniform int ScreenHeight;
uniform mat4 InvProjectionMatrix;

vec4 clipToView(vec4 clipPoint){
    vec4 view = InvProjectionMatrix* clipPoint;
    view = view/view.w;

    return view;
}

vec4 screenToView(vec4 screenPoint){
   vec2 ScreenSize = vec2(ScreenWidth,ScreenHeight);
   vec2 texCoord = screenPoint.xy/ScreenSize.xy;

   vec4 clip =  vec4(vec2(texCoord.x,texCoord.y)*2.0-1.0,screenPoint.z,screenPoint.w);

   return clipToView(clip);
}

vec3 IntersectLinePlane(vec3 A,vec3 B,float Distance){
   vec3 normal = vec3(0.0, 0.0, 1.0);

   vec3 ab =  B - A;

    float t = (Distance - dot(normal, A)) / dot(normal, ab);

     vec3 result = A + t * ab;

     return result;
}

void main(){
   const vec3 eyePos = vec3(0.0);

   uint tileSize = ScreenWidth/gl_NumWorkGroups.x;

   uint tileIndex = gl_WorkGroupID.x +
                     gl_WorkGroupID.y * gl_NumWorkGroups.x +
                     gl_WorkGroupID.z * (gl_NumWorkGroups.x * gl_NumWorkGroups.y);

   //screenspace的XY
   vec4 maxPoint_SS = vec4(vec2(gl_WorkGroupID.x + 1, gl_WorkGroupID.y + 1) * tileSize, -1.0, 1.0); 
   vec4 minPoint_SS = vec4(gl_WorkGroupID.xy * tileSize, -1.0, 1.0);

   //获取viewspace的XY
   vec3 maxPoint_VS = screenToView(maxPoint_SS).xyz;
   vec3 minPoint_VS = screenToView(minPoint_SS).xyz;

   //指数切割 获取distance
   float tileNear  = -NearPlane * pow(FarPlane/ NearPlane, gl_WorkGroupID.z/float(gl_NumWorkGroups.z));
   float tileFar   = -NearPlane * pow(FarPlane/ NearPlane, (gl_WorkGroupID.z + 1) /float(gl_NumWorkGroups.z));

    //获取近平面和原平面
    vec3 minPointNear =  IntersectLinePlane(eyePos, minPoint_VS, tileNear );
    vec3 minPointFar  =  IntersectLinePlane(eyePos, minPoint_VS, tileFar );
    vec3 maxPointNear =  IntersectLinePlane(eyePos, maxPoint_VS, tileNear );
    vec3 maxPointFar  =  IntersectLinePlane(eyePos, maxPoint_VS, tileFar );

    vec3 minPointAABB = min(min(minPointNear, minPointFar),min(maxPointNear, maxPointFar));
    vec3 maxPointAABB = max(max(minPointNear, minPointFar),max(maxPointNear, maxPointFar));

    cluster[tileIndex].minPoint = vec4(minPointAABB,0.0);
    cluster[tileIndex].maxPoint = vec4(maxPointAABB,0.0);
}
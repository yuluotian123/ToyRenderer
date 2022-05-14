#version 430 core
uniform float Metallic;
uniform float Roughness;
uniform bool useMetalMap;
uniform bool useRoughMap;
uniform bool useNormalMap;
uniform bool useMetalRoughMap;
uniform bool useIBL;

uniform sampler2D DiffuseMap0;
uniform sampler2D NormalMap0;
uniform sampler2D MetalMap0;
uniform sampler2D RoughMap0;
uniform sampler2D MetalRoughMap0;//实际上是unknownmap
uniform samplerCube irradianceMap;
uniform samplerCube specularMap;
uniform sampler2D brdfLUT;

uniform vec3 CameraPos;
uniform float NearPlane;
uniform float FarPlane;
uniform int GridSizeX;
uniform int GridSizeY;
uniform int GridSizeZ;
uniform int TileSize;
uniform float Scale;
uniform float Bias;

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform float LightIntensity;

//注意内存对齐！！！
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

in V_Out{
in  vec2 TexCoord;
in  vec3 WorldPos;
in  vec3 Normal;
in  mat3 TBN;
} f_in;

struct voAABB{
    vec4 minPoint;
	vec4 maxPoint;
};

layout(std430,binding = 1) buffer clusterAABB{
  voAABB cluster[];
};

const float PI = 3.14159265359;

out vec4 FragColor;

//将法线贴图转换成法线 疑似不正确
vec3 getNormalfromNormalMap(){
  vec3 tangentNormal = texture( NormalMap0,f_in.TexCoord).xyz * 2.0 - 1.0;
  
  return normalize(f_in.TBN * tangentNormal);
}

//获取线性深度
float linearDepth(float depthSample){
    float depthRange = 2.0 * depthSample - 1.0;
    //从ndc变换到view space
    float linear = 2.0 * NearPlane * FarPlane / (FarPlane + NearPlane - depthRange * (FarPlane - NearPlane));

    return linear;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

vec3 DirectPBR(vec3 N,vec3 V,vec3 albedo,float rough,float metal){
         vec3 color = vec3(0.0f);

         vec3 F0 = vec3(0.04f); 
         F0 = mix(F0, albedo, metal);

         vec3 radiance = LightColor*LightIntensity;

         vec3 L = normalize(LightDirection);
         L = -L;
         vec3 H = normalize(V + L);

         float NDF = DistributionGGX(N, H, rough);   
         float G = GeometrySmith(N, V, L, rough);      
         vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

         vec3 numerator    = NDF * G * F; 
         float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
         vec3 specular = numerator / denominator;

          vec3 kS = F;
          vec3 kD = vec3(1.0) - kS;
          kD *= 1.0 - metal;

          float NdotL = max(dot(N, L), 0.0);  

          color += (kD * albedo / PI + specular) * radiance * NdotL; 

          return color;
}


vec3 calcPointLight(uint lightIndex,vec3 WorldPos,vec3 N,vec3 V,vec3 albedo,float rough,float metal) {
   PointLight p  = plight[lightIndex];
   vec3 color = vec3(0.0f);

   vec3 F0 = vec3(0.04f); 
   F0 = mix(F0, albedo, metal);

   float distance = length(p.position.xyz - WorldPos);
   float attenuation = pow(clamp(1 - pow((distance /p.radius), 4.0), 0.0, 1.0), 2.0)/(1.0  + (distance * distance) );//这个算法不好
   vec3 radiance = 1000*p.color.rgb *p.intensity *attenuation;

   vec3 L = normalize(p.position.xyz - WorldPos);
   vec3 H = normalize(V + L);

   float NDF = DistributionGGX(N, H, rough);   
   float G = GeometrySmith(N, V, L, rough);      
   vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

   vec3 numerator    = NDF * G * F; 
   float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
   vec3 specular = numerator / denominator;

   vec3 kS = F;
   vec3 kD = vec3(1.0) - kS;
   kD *= 1.0 - metal;

   float NdotL = max(dot(N, L), 0.0);  

   color += (kD * albedo / PI + specular) * radiance * NdotL; 

   return color;
}

vec3 PointPBR(vec3 WorldPos,vec3 N,vec3 V,vec3 albedo,float rough,float metal){
     vec3 color = vec3(0.0f,0.0f,0.0f);

    //获取z上所处的tile
    uint zTile     = uint(max(log2(linearDepth(gl_FragCoord.z)) * Scale + Bias, 0.0));
    //获取xy上所处的tile
    uvec3 tiles    = uvec3( uvec2( gl_FragCoord.xy /TileSize), zTile);
    uint tileIndex = tiles.x +
                     GridSizeX * tiles.y +
                     (GridSizeX * GridSizeY) * tiles.z;  

    uint lightCount = Lightgrid[tileIndex].count;
    uint lightOffset = Lightgrid[tileIndex].offset;

    for(uint i = 0; i < lightCount; i++){
         uint lightIndex =  LightIndexList[lightOffset + i];  
         color += calcPointLight(lightIndex,WorldPos,N,V,albedo,rough,metal);
    }
    return color;
}

vec3 IBLPBR(vec3 N,vec3 V,vec3 R,vec3 albedo,float rough,float metal){
     vec3 irradiance = texture(irradianceMap, N).rgb;

     vec3 F0 = vec3(0.04f); 
     F0 = mix(F0, albedo, metal);

     vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, rough);

     vec3 kS = F;
     vec3 kD = 1.0 - kS;
     kD *= 1.0 - metal;	

     vec3 diffuse  = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(specularMap, R,  rough * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), rough)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = kD * diffuse + specular;

    return ambient;
}

void main()
{
     vec2 tex = f_in.TexCoord;
	 vec3 albedo = pow(texture(DiffuseMap0,tex).rgb,vec3(2.2));

     vec3 N = normalize(f_in.Normal);
     vec3 V = normalize(CameraPos - f_in.WorldPos);
     vec3 R = reflect(-V, N); 

	 float rough = Roughness;
	 float metal = Metallic;

	 if(useNormalMap)
	 N = getNormalfromNormalMap();
	 if(useRoughMap){
     rough = texture(RoughMap0,tex).r;
     }
	 if(useMetalMap){
	 metal = texture(MetalMap0,tex).r;
     }
     if(useMetalRoughMap){
     metal  =  texture(MetalRoughMap0,tex).b;
     rough = texture(MetalRoughMap0,tex).g;
     }

     vec3 color = DirectPBR(N,V,albedo,rough,metal);

     color += PointPBR(f_in.WorldPos,N,V,albedo,rough,metal);

     if(useIBL)
     color += IBLPBR(N,V,R,albedo,rough,metal);
     else
      color += vec3(0.025)* albedo;

      // HDR tonemapping
      color = color / (color + vec3(1.0));
      // gamma correct
      color = pow(color, vec3(1.0/2.2)); 

	  FragColor = vec4(color ,1.f);
}
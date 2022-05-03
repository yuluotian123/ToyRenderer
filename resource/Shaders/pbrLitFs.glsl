#version 430 core
uniform sampler2D DiffuseMap0;
uniform sampler2D NormalMap0;
uniform sampler2D MetalMap0;
uniform sampler2D RoughMap0;
uniform sampler2D UnKnownMap0;//实际上是metalroughmap

uniform float Metallic;
uniform float Roughness;
uniform bool useMetalMap;
uniform bool useRoughMap;
uniform bool useNormalMap;
uniform bool useUnKnownMap;

uniform vec3 CameraPos;
uniform float ScreenWidth;
uniform float ScreenHeight;

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform float LightIntensity;

in V_Out{
in  vec2 TexCoord;
in  vec3 WorldPos;
in  vec3 Normal;
in  mat3 TBN;
} f_in;


const float PI = 3.14159265359;

out vec4 FragColor;

vec3 getNormalfromNormalMap(){
  vec3 tangentNormal = texture( NormalMap0,f_in.TexCoord).xyz * 2.0 - 1.0;
  
  return normalize(f_in.TBN * tangentNormal);
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

void main()
{
     vec2 tex = f_in.TexCoord;
	 vec3 albedo = pow(texture(DiffuseMap0,tex).rgb,vec3(2.2));

     vec3 N = normalize(f_in.Normal);
     vec3 V = normalize(CameraPos - f_in.WorldPos);

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
     if(useUnKnownMap){
     metal  =  texture(UnKnownMap0,tex).b;
     rough = texture(UnKnownMap0,tex).g;
     }

     vec3 color =  DirectPBR(N,V,albedo,rough,metal);
      
      // HDR tonemapping
      color = color / (color + vec3(1.0));
      // gamma correct
      color = pow(color, vec3(1.0/2.2)); 

	 FragColor = vec4(color,1.f);
}
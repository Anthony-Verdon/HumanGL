#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 uCamPos;

struct PointLight
{
    vec3 pos;
    vec3 color;
    float intensity;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float intensity;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 color;
    float intensity;
};

uniform PointLight uPointLights[4];
uniform DirectionalLight uDirectionalLight;
uniform SpotLight uSpotLight;

struct Material
{
    vec4 baseColorFactor;
    vec3 emissiveFactor;
    float metallicFactor;
    float roughnessFactor;
    float ambientOcclusion;

};

uniform int uMaterialIndex;
uniform bool uUseBaseColorTexture;
uniform sampler2D uBaseColorTexture;

uniform Material uMaterials[100];

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughnessFactor)
{
    float a = roughnessFactor * roughnessFactor;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return (num / denom);
}


float GeometrySchlickGGX(float NdotV, float roughnessFactor)
{
    float r = roughnessFactor + 1.0;
    float k = r * r / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return (num / denom);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughnessFactor)
{
    float NdotV = max(dot(N, V), 0);
    float NdotL = max(dot(N, L), 0);
    
    float ggx1 = GeometrySchlickGGX(NdotV, roughnessFactor);
    float ggx2 = GeometrySchlickGGX(NdotL, roughnessFactor);

    return (ggx1 * ggx2);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CalculatePointLight(vec3 N, vec3 V, vec3 F0, vec3 baseColor, PointLight pointLight)
{
    vec3 L = normalize(pointLight.pos - WorldPos);
    vec3 H = normalize(V + L);

    float distance = length(pointLight.pos - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = pointLight.color * attenuation;

    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF = DistributionGGX(N, H, uMaterials[uMaterialIndex].roughnessFactor);
    float G = GeometrySmith(N, V, L, uMaterials[uMaterialIndex].roughnessFactor);
    vec3 num = NDF * G * F;
    float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = num / denom;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - uMaterials[uMaterialIndex].metallicFactor);

    float NdotL = max(dot(N, L), 0.0);
    return ((kD * baseColor / PI + specular) * radiance * pointLight.color * pointLight.intensity * NdotL);
}

vec3 CalculateDirectionalLight(vec3 N, vec3 V, vec3 F0, vec3 baseColor, DirectionalLight directionalLight)
{
    vec3 L = normalize(-directionalLight.direction);
    vec3 H = normalize(V + L);

    vec3 radiance = directionalLight.color;

    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF = DistributionGGX(N, H, uMaterials[uMaterialIndex].roughnessFactor);
    float G = GeometrySmith(N, V, L, uMaterials[uMaterialIndex].roughnessFactor);
    vec3 num = NDF * G * F;
    float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = num / denom;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - uMaterials[uMaterialIndex].metallicFactor);

    float NdotL = max(dot(N, L), 0.0);
    return ((kD * baseColor / PI + specular) * radiance * directionalLight.color * directionalLight.intensity * NdotL);
}

vec3 CalculateSpotLight(vec3 N, vec3 V, vec3 F0, vec3 baseColor, SpotLight spotLight)
{
    vec3 L = normalize(spotLight.position - WorldPos);
    vec3 H = normalize(V + L);

    float distance = length(spotLight.position - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = spotLight.color * attenuation;

    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF = DistributionGGX(N, H, uMaterials[uMaterialIndex].roughnessFactor);
    float G = GeometrySmith(N, V, L, uMaterials[uMaterialIndex].roughnessFactor);
    vec3 num = NDF * G * F;
    float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = num / denom;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - uMaterials[uMaterialIndex].metallicFactor);

    float NdotL = max(dot(N, L), 0.0);
    vec3 result = (kD * baseColor / PI + specular) * radiance * spotLight.color * spotLight.intensity * NdotL;

    float theta     = dot(L, normalize(-spotLight.direction));
    float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0); 

    return (result * intensity);
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(uCamPos - WorldPos);

    vec3 baseColor;
    if (uUseBaseColorTexture)
        baseColor = vec3(texture(uBaseColorTexture, TexCoord));
    else
        baseColor = vec3(uMaterials[uMaterialIndex].baseColorFactor);

    vec3 F0 = mix(vec3(0.04), baseColor, uMaterials[uMaterialIndex].metallicFactor);
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 4; i++)
    {
        Lo += CalculatePointLight(N, V, F0, baseColor, uPointLights[i]);
    }
    Lo += CalculateDirectionalLight(N, V, F0, baseColor, uDirectionalLight);
    
    Lo += CalculateSpotLight(N, V, F0, baseColor, uSpotLight);

    vec3 ambient = vec3(0.03) * baseColor;
    vec3 color = ambient + Lo * uMaterials[uMaterialIndex].ambientOcclusion + uMaterials[uMaterialIndex].emissiveFactor;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    
    FragColor = vec4(color, 1.0);
}
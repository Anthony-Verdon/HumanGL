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

uniform PointLight uPointLights[4];

uniform vec3 uBaseColor;
uniform vec3 uEmissiveColor;
uniform float uMetallic;
uniform float uRoughness;
uniform float uAmbientOcclusion;

uniform bool uUseBaseColorTexture;
uniform sampler2D uBaseColorTexture;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return (num / denom);
}


float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = r * r / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return (num / denom);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0);
    float NdotL = max(dot(N, L), 0);
    
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

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
    float NDF = DistributionGGX(N, H, uRoughness);
    float G = GeometrySmith(N, V, L, uRoughness);
    vec3 num = NDF * G * F;
    float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = num / denom;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - uMetallic);

    float NdotL = max(dot(N, L), 0.0);
    return ((kD * baseColor / PI + specular) * radiance * pointLight.color * pointLight.intensity * NdotL);
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(uCamPos - WorldPos);

    vec3 baseColor;
    if (uUseBaseColorTexture)
        baseColor = vec3(texture(uBaseColorTexture, TexCoord));
    else
        baseColor = uBaseColor;

    vec3 F0 = mix(vec3(0.04), baseColor, uMetallic);
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 4; i++)
    {
        Lo += CalculatePointLight(N, V, F0, baseColor, uPointLights[i]);
    }

    vec3 ambient = vec3(0.03) * baseColor;
    vec3 color = ambient + Lo * uAmbientOcclusion + uEmissiveColor;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));
    
    FragColor = vec4(color, 1.0);
}
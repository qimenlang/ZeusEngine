#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

#define saturate(x) clamp(x, 0.0, 1.0)

// PBR Mat
struct Brdf {
    vec3 albedo;
    float roughness;
    float metallic;
    float ao;
}; 
uniform Brdf brdf;

// lights
struct Light{
    vec3 pos;
    vec3 color;
};
uniform Light lights[4];

// subsurface scattering
struct Subsurface{
    float thickness;
    vec3 color;
    float power;
    float scale;
    float distortion;
};

uniform Subsurface subsurface;

uniform vec3 camPos;

const float PI = 3.14159265359;

// Normal Distribution : statistically approximates the  
// rate of microfacets normal exactly aligned to the halfway vector 
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

// describes the self-shadowing property of the microfacets.
// When a surface is relatively rough, the surface's microfacets can overshadow other microfacets reducing the light the surface reflects
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

// fresnel equation:
// describes the ratio of light that gets reflected over the light that gets refracted,
// which varies over the angle we're looking at a surface
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 SubsurfaceScattering(vec3 N, vec3 L, vec3 V, float thickness, vec3 color, float power, float scale, float distortion)
{
    vec3 distortedLightDir = L + N * distortion;  // 光线扭曲
    float H = dot(V, -distortedLightDir);           // 半角向量计算
    float scattering = pow(saturate(-H), power) * scale * thickness;
    return scattering * color;
}

vec3 filamentSubsurfaceScattering(vec3 N, vec3 L, vec3 V, float thickness, vec3 color, float power, float scale)
{
    float NoL = saturate(dot(N, L));
    float scatterVoH = saturate(dot(V, -L));  
    float forwardScatter = exp2(scatterVoH *power - power);
    float backScatter = saturate(NoL * thickness + (1.0 - thickness)) * 0.5;
    float ss = mix(backScatter, 1.0, forwardScatter) * (1.0 - thickness);
    return color * (ss / PI ) * scale;
}

void main()
{	
    vec3 N = normalize(normal);
    vec3 V = normalize(camPos-fragPos);

    // metal F0 should be 0.04
    vec3 F0 = vec3(0.04);
    // metallic workflow : use albedo as F0 
    F0 =mix(F0,brdf.albedo,brdf.metallic);

    // rendering equation
    vec3 Lo = vec3(0.0);
    for(int i=0;i<4;i++){
        // calculate per-light radiance
        vec3 L = normalize(lights[i].pos - fragPos);
        vec3 H = normalize(L+V);
        float distance = length(lights[i].pos - fragPos);
        float attenuation = 1.0/(distance*distance);
        vec3 radiance = lights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N,H,brdf.roughness);
        float G = GeometrySmith(N,V,L,brdf.roughness);
        vec3 F = fresnelSchlick(clamp(dot(H,V),0.0,1.0),F0);

        vec3 numerator = NDF * G * F; 
        // + 0.0001 to prevent divide by zero
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // energy conservation diffuse+specular=1.0f
        vec3 kD = vec3(1.0)-kS;
        // pure metals have no diffuse light
        kD *= 1.0 - brdf.metallic;
        // scale light by cosTheta
        float cosTheta  = max(dot(N, L), 0.0);
        // add to outgoing radiance Lo
        Lo += (kD * brdf.albedo / PI + specular) * radiance * cosTheta; 

        // subsurface scattering
        // Lo += SubsurfaceScattering(N, L, V, subsurface.thickness, subsurface.color, subsurface.power, subsurface.scale, subsurface.distortion)/4.0;
        Lo += filamentSubsurfaceScattering(N, L, V, subsurface.thickness, subsurface.color, subsurface.power, subsurface.scale)/4.0;
    }

    vec3 ambient = vec3(0.03)*brdf.albedo*brdf.ao;
    
    vec3 color = ambient + Lo;
    // tone mapping : form HDR to LDR
    color = color/(color+vec3(1.0));
    // gama correction
    color = pow(color,vec3(1.0/2.2));

    // color = subsurface.color ;
    FragColor = vec4(color,1.0);
}
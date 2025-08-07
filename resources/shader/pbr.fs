#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

// PBR Mat
struct Material {
    vec3 albedo;
    float roughness;
    float metallic;
    float ao;
}; 
uniform Material mat;

// lights
struct Light{
    vec3 pos;
    vec3 color;
};
uniform Light lights[4];

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

void main()
{	
    vec3 N = normalize(normal);
    vec3 V = normalize(camPos-fragPos);

    // metal F0 should be 0.04
    vec3 F0 = vec3(0.04);
    // metallic workflow : use albedo as F0 
    F0 =mix(F0,mat.albedo,mat.metallic);

    // rendering equation
    vec3 Lo = vec3(0.0);
    for(int i=0;i<4;i++){
        // calculate per-light radiance
        vec3 L = normalize(lights[i].pos - fragPos);
        vec3 H = normalize(L+V);
        float distance = length(lights[i].pos - fragPos);
        float attenuation = 1.0/(distance*distance);
        // scale light by cosTheta
        float cosTheta  = max(dot(N, L), 0.0);
        vec3 radiance = lights[i].color * attenuation * cosTheta;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N,H,mat.roughness);
        float G = GeometrySmith(N,V,L,mat.roughness);
        vec3 F = fresnelSchlick(clamp(dot(H,V),0.0,1.0),F0);

        vec3 numerator = NDF * G * F; 
        // + 0.0001 to prevent divide by zero
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // energy conservation diffuse+specular=1.0f
        vec3 kD = vec3(1.0)-F;
        // pure metals have no diffuse light
        kD *= 1.0 - mat.metallic;
        // add to outgoing radiance Lo
        Lo += (kD * mat.albedo / PI + specular) * radiance ; 
    }

    vec3 ambient = vec3(0.03)*mat.albedo*mat.ao;
    
    vec3 color = ambient + Lo;
    // tone mapping : form HDR to LDR
    color = color/(color+vec3(1.0));
    // gama correction
    color = pow(color,vec3(1.0/2.2));

    // color = N;
    FragColor = vec4(color,1.0);
}
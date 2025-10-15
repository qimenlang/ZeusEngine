#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

// PBR Mat texture
uniform sampler2D albedo1;
uniform sampler2D normal1;
uniform sampler2D roughness1;
uniform sampler2D metallic1;
uniform sampler2D ao1;

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
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anyways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normal1, texCoord).xyz * 2.0 - 1.0;

    //dFdx/dFdy 屏幕空间x/y方向的偏导数,相邻片段间的差值
    //Q1/Q2 片段位置(fragPos)在屏幕空间的梯度，表示表面切向量的近似
    vec3 Q1  = dFdx(fragPos);
    vec3 Q2  = dFdy(fragPos);
    // st1/st2 纹理坐标(texCoord)在屏幕空间的梯度
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{	
    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos-fragPos);

    // metal F0 should be 0.04
    vec3 F0 = vec3(0.04);
    vec3 albedo = texture(albedo1, texCoord).rgb;
    float roughness = texture(roughness1, texCoord).r;
    float metallic = texture(metallic1, texCoord).r;
    float ao = texture(ao1, texCoord).r;

    // metallic workflow : use albedo as F0 
    F0 =mix(F0,albedo,metallic);

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
        float NDF = DistributionGGX(N,H,roughness);
        float G = GeometrySmith(N,V,L,roughness);
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
        kD *= 1.0 - metallic;
        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance ; 
    }

    vec3 ambient = vec3(0.03)*albedo*ao;
    
    vec3 color = ambient + Lo;
    // tone mapping : form HDR to LDR
    color = color/(color+vec3(1.0));
    // gama correction
    color = pow(color,vec3(1.0/2.2));

    FragColor = vec4(color,1.0);
}
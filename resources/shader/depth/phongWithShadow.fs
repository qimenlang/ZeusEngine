#version 330 core
out vec4 FragColor;
in vec3 fragPos;
in vec4 lightSpaceFragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuse1;
uniform sampler2D shadowMap1;
uniform vec3 viewPos;

struct Material {
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

vec2 texelSize = 1.0 / textureSize(shadowMap1, 0);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 透视除法到NDC
    vec3 shadowCoords =fragPosLightSpace.xyz/fragPosLightSpace.w;
    // 视口变换
    shadowCoords = shadowCoords*0.5+0.5;
    float actualDepth = shadowCoords.z;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.05;
    int pcfWidth = 5;
    float shadow = 0.0f;
    float depth = texture(shadowMap1, shadowCoords.xy).r;
    shadow =  actualDepth -bias > depth ? 1.0 : 0.0;
    return shadow;
}

float ShadowCalculationPCF(vec4 fragPosLightSpace)
{
    // 透视除法到NDC
    vec3 shadowCoords =fragPosLightSpace.xyz/fragPosLightSpace.w;
    // 视口变换
    shadowCoords = shadowCoords*0.5+0.5;
    float actualDepth = shadowCoords.z;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.05;
    int pcfWidth = 5;
    float shadow = 0.0f;

    for(int x = -pcfWidth; x <= pcfWidth; ++x)
    {
        for(int y = -pcfWidth; y <= pcfWidth; ++y)
        {
            float pcfDepth = texture(shadowMap1, shadowCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += actualDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow/=(pcfWidth*pcfWidth);

    return shadow;
}

void main()
{
    vec3 sampleDiffuse = vec3(texture(diffuse1, texCoord));
    vec3 sampleShadow = vec3(texture(shadowMap1, texCoord).r);

    vec3 ambient = sampleDiffuse * light.ambient;

    vec3 norm = normalize(normal);
    // vec3 norm = normalize(vec3(texture(normal1,texCoord)));
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse= diff * sampleDiffuse * light.diffuse;
    vec3 viewDir = normalize(viewPos-fragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    float inShadow = ShadowCalculation(lightSpaceFragPos);
    
    vec3 result = ambient+(diffuse+specular)*(1.0-inShadow); 

    FragColor = vec4(result,1.0);
}
#version 330 core
out vec4 FragColor;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuse1;
uniform sampler2D normal1;
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

void main()
{
    vec3 sampleDiffuse = vec3(texture(diffuse1, texCoord));

    vec3 ambient = sampleDiffuse * light.ambient;

    vec3 norm = normalize(normal);
    // vec3 norm = normalize(vec3(texture(normal1,texCoord)));
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse= diff * sampleDiffuse * light.diffuse;

    vec3 viewDir = normalize(viewPos-fragPos);
    // 使用半程向量
    vec3 halfDir = normalize(viewDir+lightDir);
    float spec = pow(max(dot(halfDir,norm),0.0),material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result = diffuse+ambient+specular; 

    FragColor = vec4(result,1.0);
}
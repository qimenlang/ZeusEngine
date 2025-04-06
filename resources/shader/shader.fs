#version 330 core
out vec4 FragColor;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
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
    // FragColor = mix(texture(texture_diffuse1, texCoord), texture(texture_diffuse2, texCoord), 0.2);
    // FragColor = vec4(normal,1.0);
    // FragColor = vec4(texCoord.x,0.0,0.0,1.0);

    vec3 ambient = material.ambient * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse= diff * material.diffuse * light.diffuse;

    vec3 viewDir = normalize(viewPos-fragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec * material.specular * light.specular;

    vec3 result1 = (diffuse+ambient+specular)*vec3(texture(texture_diffuse1, texCoord));
    vec3 result2 = diffuse+ambient+specular; 

    //  if(gl_FragCoord.y > 540)
    //     FragColor = vec4(result1,1.0);
    // else
        //FragColor = vec4(normal,1.0);

    FragColor = vec4(result2,1.0);
}
#version 330 core
out vec4 FragColor;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    // FragColor = mix(texture(texture_diffuse1, texCoord), texture(texture_diffuse2, texCoord), 0.2);
    // FragColor = vec4(normal,1.0);
    // FragColor = vec4(texCoord.x,0.0,0.0,1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse= diff * lightColor;

    vec3 result1 = (diffuse+ambient)*vec3(texture(texture_diffuse1, texCoord));
    vec3 result2 = (diffuse+ambient)*vec3(1,1,1); 

     if(gl_FragCoord.y > 540)
        FragColor = vec4(result1,1.0);
    else
        //FragColor = vec4(normal,1.0);
        FragColor = vec4(result2,1.0);
}
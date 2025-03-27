#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main()
{
    // FragColor = mix(texture(texture_diffuse1, texCoord), texture(texture_diffuse2, texCoord), 0.2);
    // FragColor = vec4(normal,1.0);
    // FragColor = vec4(texCoord.x,0.0,0.0,1.0);
    FragColor = texture(texture_diffuse1, texCoord);
}
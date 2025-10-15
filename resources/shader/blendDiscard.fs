#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D diffuse1;

void main()
{
    vec4 texColor =texture(diffuse1, texCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
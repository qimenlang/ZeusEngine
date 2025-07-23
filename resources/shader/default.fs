#version 330 core
out vec4 FragColor;

uniform vec3 MatColor;

void main()
{
    FragColor = vec4(MatColor,1.0);
}
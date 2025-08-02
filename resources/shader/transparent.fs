#version 330 core
out vec4 FragColor;

uniform vec3 MatColor;
uniform float alpha;

void main()
{
    FragColor = vec4(MatColor,alpha);
}
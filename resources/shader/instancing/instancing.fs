#version 330 core
out vec4 FragColor;

in vec3 fColor;
in vec2 offset;

void main()
{
    FragColor = vec4(offset,0.0, 1.0);
}
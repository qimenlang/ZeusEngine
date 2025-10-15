#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in mat4 instanceModel;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texCoord = aTexCoords;
    gl_Position = projection*view*instanceModel*vec4(aPos,1.0); 
}
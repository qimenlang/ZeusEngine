#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec4 lightSpaceFragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;


void main()
{
    fragPos  = vec3(model*vec4(aPos,1.0));
    lightSpaceFragPos = lightSpaceMatrix*vec4(fragPos,1.0);
    //将法线从模型空间正确地变换到世界空间，同时保证法线在变换后仍然垂直于表面
    normal = mat3(transpose(inverse(model))) * aNormal;
    texCoord = aTexCoord;
    gl_Position = projection*view*model*vec4(aPos,1.0);
}
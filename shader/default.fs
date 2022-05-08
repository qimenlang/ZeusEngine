#version 330 core
out vec4 FragColor;
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
in vec2 TexCoord;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, TexCoord);
    vec3 color = tex.rgb*lightColor;
    FragColor = vec4(color,tex.a);
}
#version 330 core
out vec4 FragColor;

in vec2 texCoord;

// uniform sampler2D screenTexture;

void main()
{
    // vec3 color = texture(screenTexture, texCoord).rgb;
    FragColor = vec4(texCoord.x,texCoord.y,0.0, 1.0);
}
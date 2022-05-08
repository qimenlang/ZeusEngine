#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, TexCoord);
    //ambient = Ka * I
    float ambientCoeff = 0.1;
    vec3 ambient = ambientCoeff * lightColor;

    //diffuse  = Kd * I/r^2 *max(0,n*l)
    // 漫反射系数、光照的强度、半径的平方、0和 法线与光线反方向 cos的最大值

    //specular  = Ks * I/r^2 * max(0,h*n)  
    // h 是半程向量， h = (v+l) ;视线与光线方向相加得到半程向量


    vec3 result = ambient * tex.rgb;
    FragColor = vec4(normal, 1.0);
}
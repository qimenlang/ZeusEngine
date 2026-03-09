// Simple sine wave ripple
// ShaderToy compatible

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453);
}

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hash(i);
    float b = hash(i + vec2(1.0,0.0));
    float c = hash(i + vec2(0.0,1.0));
    float d = hash(i + vec2(1.0,1.0));

    vec2 u = f*f*(3.0-2.0*f);

    return mix(a,b,u.x) +
           (c-a)*u.y*(1.0-u.x) +
           (d-b)*u.x*u.y;
}

float rotorTurb(float height, vec2 uv){
    float turbScale = 8.0;
    float turbSpeed = 1.5;
    float turb = noise(uv * turbScale + iTime * turbSpeed);
    turb -= 0.5; // centered
    height += turb * 0.03;
    return height;
}


float sinWave(vec2 uv){
    // 距离中心
    float r = length(uv);

    // 波参数
    float frequency = 25.0;
    float speed = 2.0;
    float amplitude = 0.02;

    // 单一sin波
    float wave = sin(r * frequency - iTime * speed);

    // 位移
    float height = wave * amplitude;
    height = rotorTurb(height, uv); // add some turbulence to the wave height
    
    // 用波纹改变亮度
    wave = 0.5 + wave * 0.5;

    return wave;
}

float gerstnerWave(vec2 uv) {
        // wave parameters
    float amplitude = 1.0;
    float wavelength = 0.04;
    float speed = 0.05;

    float k = 2.0 * 3.14159 / wavelength;
    float w = speed * k;

    // wave 方向:平行 
    // vec2 dir = normalize(vec2(1.0, 1.0));
    // wave 方向：径向
    vec2 dir = normalize(uv);

    float phase = k * dot(dir, uv) - w * iTime;

    // Gerstner displacement
    vec2 displacement;
    displacement.x = dir.x * amplitude * cos(phase);
    displacement.y = dir.y * amplitude * cos(phase);

    float height = amplitude * sin(phase);
    height = rotorTurb(height, uv); // add some turbulence to the wave height

    // apply displacement
    vec2 pos = uv + displacement;

    // simple shading
    float light = 0.5 + height * 0.5;
    return light;
}

float oceanHeight(vec2 uv){
    int type = 1; // 0: sin wave, 1: gerstner wave
    if(0==type) 
        return sinWave(uv);
    else if(1==type)
        return gerstnerWave(uv);
    else
        return 0.0;
}

float getHeight(vec2 uv)
{
    // float light = sinWave(uv);
    float height1 = oceanHeight(uv-vec2(0.5,0.7));
    float height2 = oceanHeight(uv-0.3);
    float height3 = oceanHeight(uv-vec2(0.7,0.3));
    float height = (height1 + height2 + height3) / 3.0;
    return height;
}

//--------------------------------
// normal
//--------------------------------

vec3 getNormal(vec2 uv)
{
    float e=0.002;

    float h=getHeight(uv);
    float hx=getHeight(uv+vec2(e,0));
    float hy=getHeight(uv+vec2(0,e));

    return normalize(vec3(h-hx,e,h-hy));
}

//--------------------------------
// fresnel
//--------------------------------

float fresnel(vec3 n, vec3 v)
{
    return pow(1.0-max(dot(n,v),0.0),5.0);
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // 归一化坐标
    vec2 uv = fragCoord / iResolution.xy;

    // 保持比例
    uv.x *= iResolution.x / iResolution.y;    
    // 简单颜色
    vec3 waterColor = vec3(0, 0, 1);
 
    float height = getHeight(uv);

    // 着色
    vec3 n=getNormal(uv);

    vec3 viewDir=normalize(vec3(0.0,1,0.0));

    vec3 lightDir=normalize(vec3(0.4,1.0,0.3));

    //--------------------------------
    // lighting
    //--------------------------------

    float diff=max(dot(n,lightDir),0.0);

    vec3 halfDir=normalize(lightDir+viewDir);

    float spec=pow(max(dot(n,halfDir),0.0),80.0);

    float F=fresnel(n,viewDir);

    //--------------------------------
    // base water
    //--------------------------------

    vec3 deep=vec3(0.0,0.25,0.45);
    vec3 shallow=vec3(0.0,0.45,0.75);

    vec3 water=mix(deep,shallow,diff);

    //vec3 color = waterColor * height;

    vec3 color=water;

    color += spec*vec3(1.0);

    // color=mix(color,vec3(1.0),foam*0.6);

    // color+=mist*vec3(1.0);

    color+=F*vec3(0.4,0.6,0.8);

    fragColor = vec4(color, 1.0);
}
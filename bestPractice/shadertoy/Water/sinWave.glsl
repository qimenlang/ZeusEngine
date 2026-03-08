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

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // 归一化坐标
    vec2 uv = fragCoord / iResolution.xy;

    // 保持比例
    uv.x *= iResolution.x / iResolution.y;    
    // 简单颜色
    vec3 waterColor = vec3(0, 0, 1);

    // 居中
    // uv -= 0.5;
    
    // float light = sinWave(uv);
    float light1 = gerstnerWave(uv-0.5);
    float light2 = gerstnerWave(uv-0.3);
    float light3 = gerstnerWave(uv-vec2(0.4,0.7));
    float light = (light1 + light2 + light3) / 3.0;

    vec3 color = waterColor * light;

    fragColor = vec4(color, 1.0);
}
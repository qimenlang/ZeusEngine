// Gerstner wave + rotor turbulence
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

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord / iResolution.xy;

    uv -= 0.5;
    uv.x *= iResolution.x / iResolution.y;

    //--------------------------------
    // Gerstner wave
    //--------------------------------
    float amplitude = 0.05;
    float wavelength = 0.4;
    float speed = 2.0;

    float k = 2.0 * 3.14159 / wavelength;
    float w = speed * k;

    vec2 dir = normalize(vec2(1.0,0.3));

    float phase = k * dot(dir, uv) - w * iTime;

    vec2 displacement;
    displacement = dir * amplitude * cos(phase);

    float height = amplitude * sin(phase);

    //--------------------------------
    // Rotor turbulence
    //--------------------------------

    float turbScale = 8.0;
    float turbSpeed = 1.5;

    float turb = noise(uv * turbScale + iTime * turbSpeed);

    turb -= 0.5; // centered

    height += turb * 0.03;

    //--------------------------------
    // shading
    //--------------------------------

    float light = 0.5 + height * 4.0;

    vec3 waterColor = vec3(0.0,0.45,0.75);

    vec3 color = waterColor * light;

    fragColor = vec4(color,1.0);
}
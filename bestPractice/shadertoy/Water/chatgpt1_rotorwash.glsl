// Helicopter Rotor Downwash Ocean
// ShaderToy Image Shader

#define PI 3.1415926

//--------------------------------
// hash + noise
//--------------------------------

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453123);
}

float noise(vec2 p)
{
    vec2 i=floor(p);
    vec2 f=fract(p);

    float a=hash(i);
    float b=hash(i+vec2(1,0));
    float c=hash(i+vec2(0,1));
    float d=hash(i+vec2(1,1));

    vec2 u=f*f*(3.0-2.0*f);

    return mix(a,b,u.x)
         +(c-a)*u.y*(1.0-u.x)
         +(d-b)*u.x*u.y;
}

//--------------------------------
// gerstner wave
//--------------------------------

float gerstner(vec2 p, vec2 dir, float amp, float waveLen, float speed)
{
    float k = 2.0*PI/waveLen;
    float w = speed*k;
    float phase = k*dot(dir,p)-w*iTime;
    return amp*sin(phase);
}

//--------------------------------
// ocean height
//--------------------------------

float oceanHeight(vec2 p)
{
    float h=0.0;

    // base ocean waves
    h+=gerstner(p,normalize(vec2(1.0,0.3)),0.05,0.45,1.2);
    h+=gerstner(p,normalize(vec2(-0.6,1.0)),0.03,0.3,1.5);
    h+=gerstner(p,normalize(vec2(0.8,0.2)),0.02,0.2,1.8);

    // rotor center
    vec2 center=vec2(0.0,0.0);

    float r=length(p-center);

    //--------------------------------
    // rotor ring waves
    //--------------------------------

    float ring=sin(r*40.0-iTime*8.0);
    ring*=exp(-r*4.0);

    h+=ring*0.04;

    //--------------------------------
    // rotor turbulence
    //--------------------------------

    float turb=noise(p*10.0+iTime*3.0);
    turb-=0.5;

    float mask=exp(-r*3.0);

    h+=turb*0.05*mask;

    return h;
}

//--------------------------------
// normal from height
//--------------------------------

vec3 getNormal(vec2 p)
{
    float e=0.002;

    float h=oceanHeight(p);
    float hx=oceanHeight(p+vec2(e,0));
    float hy=oceanHeight(p+vec2(0,e));

    vec3 n=normalize(vec3(h-hx,e,h-hy));

    return n;
}

//--------------------------------
// main
//--------------------------------

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 uv=fragCoord/iResolution.xy;

    uv-=0.5;
    uv.x*=iResolution.x/iResolution.y;

    //--------------------------------
    // height
    //--------------------------------

    float h=oceanHeight(uv);

    //--------------------------------
    // normal
    //--------------------------------

    vec3 n=getNormal(uv);

    //--------------------------------
    // lighting
    //--------------------------------

    vec3 lightDir=normalize(vec3(0.4,1.0,0.3));

    float diff=max(dot(n,lightDir),0.0);

    //--------------------------------
    // rotor foam region
    //--------------------------------

    float r=length(uv);

    float foam=smoothstep(0.2,0.05,r);
    foam*=smoothstep(0.4,0.1,abs(h)*10.0);

    //--------------------------------
    // water color
    //--------------------------------

    vec3 water=vec3(0.0,0.45,0.75);

    vec3 color=water*(0.3+diff*0.7);

    color+=foam*vec3(1.0);

    fragColor=vec4(color,1.0);
}
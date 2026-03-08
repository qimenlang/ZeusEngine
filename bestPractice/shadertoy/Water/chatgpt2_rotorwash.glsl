// Helicopter Downwash Ocean (advanced)
// ShaderToy Image shader

#define PI 3.1415926

//--------------------------------
// hash + noise
//--------------------------------

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453);
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

float gerstner(vec2 p, vec2 dir, float amp, float len, float speed)
{
    float k=2.0*PI/len;
    float w=k*speed;

    float phase=k*dot(p,dir)-w*iTime;

    return amp*sin(phase);
}

//--------------------------------
// kelvin wake pattern
//--------------------------------

float kelvinWake(vec2 p)
{
    float r=length(p);

    float angle=atan(p.y,p.x);

    float wake=sin(r*30.0-iTime*6.0);

    wake*=exp(-r*2.0);

    wake*=cos(angle*3.0);

    return wake*0.03;
}

//--------------------------------
// rotor vortex ring
//--------------------------------

float vortexRing(vec2 p)
{
    float r=length(p);

    float ring=sin(r*45.0-iTime*10.0);

    ring*=exp(-r*5.0);

    return ring*0.05;
}

//--------------------------------
// turbulence
//--------------------------------

float turbulence(vec2 p)
{
    float t=noise(p*12.0+iTime*4.0);
    t-=0.5;

    float r=length(p);

    float mask=exp(-r*3.0);

    return t*0.04*mask;
}

//--------------------------------
// ocean height
//--------------------------------

float oceanHeight(vec2 p)
{
    float h=0.0;

    // base ocean
    h+=gerstner(p,normalize(vec2(1.0,0.4)),0.05,0.5,1.2);
    h+=gerstner(p,normalize(vec2(-0.7,1.0)),0.03,0.35,1.4);
    h+=gerstner(p,normalize(vec2(0.6,-0.3)),0.02,0.25,1.8);

    // helicopter disturbances
    h+=vortexRing(p);
    h+=kelvinWake(p);
    h+=turbulence(p);

    return h;
}

//--------------------------------
// normal
//--------------------------------

vec3 getNormal(vec2 p)
{
    float e=0.002;

    float h=oceanHeight(p);
    float hx=oceanHeight(p+vec2(e,0));
    float hy=oceanHeight(p+vec2(0,e));

    return normalize(vec3(h-hx,e,h-hy));
}

//--------------------------------
// spray mist particles
//--------------------------------

float spray(vec2 p)
{
    float n=noise(p*20.0+iTime*5.0);

    float r=length(p);

    float mask=exp(-r*4.0);

    float droplets=smoothstep(0.7,1.0,n);

    return droplets*mask;
}

//--------------------------------
// fresnel
//--------------------------------

float fresnel(vec3 n, vec3 v)
{
    return pow(1.0-max(dot(n,v),0.0),5.0);
}

//--------------------------------
// main
//--------------------------------

void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 uv=fragCoord/iResolution.xy;

    uv-=0.5;
    uv.x*=iResolution.x/iResolution.y;

    float h=oceanHeight(uv);

    vec3 n=getNormal(uv);

    vec3 viewDir=normalize(vec3(0.0,1.2,1.0));

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

    //--------------------------------
    // foam
    //--------------------------------

    float r=length(uv);

    float foam=exp(-r*3.0)*smoothstep(0.02,0.06,abs(h));

    //--------------------------------
    // spray mist
    //--------------------------------

    float mist=spray(uv);

    //--------------------------------
    // final color
    //--------------------------------

    vec3 color=water;

    color+=spec*vec3(1.0);

    color=mix(color,vec3(1.0),foam*0.6);

    color+=mist*vec3(1.0);

    color+=F*vec3(0.4,0.6,0.8);

    fragColor=vec4(color,1.0);
}
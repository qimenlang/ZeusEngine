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
// gerstner wave
//--------------------------------

float gerstner(vec2 uv, vec2 dir, float amp, float len, float speed)
{
    float r = length(uv); 
    // 扭曲uv坐标，改变波形形状
    vec2 warp = vec2(noise(uv*3.0 + iTime),noise(uv*3.0 + iTime + 10.0));
    float bTwist = step(0.01, r);
    // uv += warp * 0.02*bTwist;

    float k=2.0*PI/len;
    float w=k*speed;

    float phase=k*dot(uv,dir)-w*iTime;

    // 随距离衰减
    float decl = exp(-r*5.0);
    amp *= decl; 

    // avoid singularity at center
    // float mask = smoothstep(0.01,0.1,r);
    // amp *= mask;

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

    wake*=exp(-r*15.0);

    wake*=cos(angle*3.0);

    return wake*0.03;
}

//--------------------------------
// rotor vortex ring
//--------------------------------

float sinWave(vec2 uv)
{
    // 距离中心
    float r = length(uv);

    // 波参数
    float frequency = 50.0;
    float speed = 2.0;
    float amplitude = 0.05;

    // 单一sin波
    float wave = sin(r * frequency - iTime * speed);

    // 随距离衰减
    // wave*=exp(-r*5.0);

    return wave*amplitude;
}

float vortexRing(vec2 uv){


    // float ring = sinWave(uv);
    // 振幅、波长、速度
    float amp = 0.05;
    // float len = clamp(0.05*r,0.02,0.05);
    float len = 0.03;
    float speed = 0.05;
    vec2 uv0 = uv-vec2(0.0,-0.1);
    vec2 dir = normalize(uv0);

    // 扭曲uv坐标，改变波形形状
    // vec2 warp = vec2(noise(uv*3.0 + iTime),noise(uv*3.0 + iTime + 10.0));
    // uv0 += warp * 0.03;
    // 噪声调整波长，增加自然感
    // len = len * (0.7 + 0.3*noise(uv0*2.0));
    // 扰动传播方向
    // float angle = noise(uv0*1.5) * 2.0 * PI;
    // vec2 dirWarp = normalize(
    //     uv0+
    //     vec2(cos(angle), sin(angle))*0.03
    // );



    float ring =  gerstner(uv0, normalize(uv0), amp, len, speed*0.8);
    float offset = 0.25;
    vec2 uv1 = uv-vec2(offset,offset);
    ring+=  gerstner(uv1, normalize(uv1), amp, len*0.8, speed);
    vec2 uv2 = uv-vec2(-offset,offset);
    ring+=  gerstner(uv2, normalize(uv2),  amp, len, speed*1.111);

    ring /= 3.0;

    float r = length(uv);
    float decl = exp(-r*5.0);
    ring *= decl; // 随距离衰减

    // uv-=0.1; 
    // float ring=  gerstner(uv, normalize(uv), 0.1, 0.1, 0.2);
    return ring;
}


//--------------------------------
// ocean height
//--------------------------------

float oceanHeight(vec2 p)
{
    float h=0.0;

    // base ocean
    // h+=gerstner(p-vec2(0.5,0.7),normalize(vec2(1.0,0.4)),0.05,0.5,0.2);
    // h+=gerstner(p,normalize(vec2(-0.7,1.0)),0.03,0.35,0.4);
    // h+=gerstner(p,normalize(vec2(0.6,-0.3)),0.02,0.25,0.6);

    // helicopter disturbances
    h+=vortexRing(p);
    // h+=kelvinWake(p); 
    // h+=turbulence(p);

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

    //uv+=vec2(sin(uv.x))*0.2;// 尝试修改形状
    uv-=0.5;
    uv.x*=iResolution.x/iResolution.y;

    float h=oceanHeight(uv);

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

    // color=mix(color,vec3(1.0),foam*0.6);

    // color+=mist*vec3(1.0);

    color+=F*vec3(0.4,0.6,0.8);

    //debug
    // color = vec3(length(uv),0,0);
    // color = vec3(uv+0.5,0);

    fragColor=vec4(color,1.0);
}
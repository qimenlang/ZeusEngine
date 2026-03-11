// shadertoy: main
#iChannel0 "rotorwash.glsl"
#iChannel1 "bufferA.glsl"
#iChannel2 "bufferB.glsl"

vec3 camPos = vec3(0.0,10.0,0.0);

const float cols = 10.0;
const float rows = 10.0;
const float totalFrames = cols * rows; // 16 帧


vec3 getCamera(vec2 uv)
{
    vec2 mouse = texture2D(iChannel2,vec2(0.5)).xy/ iResolution.xy;

    float yaw   = (mouse.x - 0.5) * 6.28318;
    float pitch = (mouse.y - 0.5) * 1.5;

    vec3 forward = normalize(vec3(
        cos(pitch) * sin(yaw),
        sin(pitch),
        cos(pitch) * cos(yaw)
    ));

    vec3 right = normalize(cross(vec3(0,1,0), forward));
    vec3 up = cross(forward, right);

    vec3 dir = normalize(
        uv.x * right +
        uv.y * up +
        1.5 * forward
    );

    return dir;
}

//--------------------------------
// fresnel
//--------------------------------

float fresnel(vec3 n, vec3 v)
{
    return pow(1.0-max(dot(n,v),0.0),5.0);
}


void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (fragCoord - 0.5*iResolution.xy)/iResolution.y;
    vec3 rayDir = getCamera(uv);

    camPos = texture2D(iChannel1, uv).rgb;

    // plane intersection
    float t = (0.0 - camPos.y) / rayDir.y;

    if(t < 0.0)
    {
        fragColor = vec4(0,0,0.7,0.0);
        return;
    }

    vec3 pos = camPos + t * rayDir;

    vec3 color = vec3(0.0);

    // quad size
    float oceanSize = 50.0;
    vec2 oceanUV = (pos.xz / oceanSize) * 0.5 + 0.5;

    vec3 n = texture2D(iChannel0, oceanUV).rgb;
    
    vec3 viewDir=normalize(camPos - pos);

    vec3 lightDir=normalize(vec3(0.4,1.0,0.3));
    lightDir=normalize(vec3(0.0,1.0,0.0));

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

    vec3 deep=vec3(0.0,0.15,0.35);
    vec3 shallow=vec3(0.0,0.55,0.85);

    // vec3 deep=vec3(0.0,0.0,1.0);
    // vec3 shallow=vec3(1.0,0.0,0.0);

    vec3 water=mix(deep,shallow,diff);

    //--------------------------------
    // final color
    //--------------------------------

    color=water;

    // color+=spec*vec3(1.0);

    // color+=F*vec3(0.4,0.6,0.8);

    fragColor = vec4(color,1.0);
}
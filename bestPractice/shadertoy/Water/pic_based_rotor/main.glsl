// shadertoy: main

// #iChannel0 "file://..//pictures//rotor2//atlas_6_6.png"
// #iChannel0 "file://..//pictures//rotor2//atlas_20_12.png"
#iChannel0 "file://..//pictures//rotor2//atlas_10_10.png"
#iChannel1 "base_ocean.glsl"

vec3 camPos = vec3(30.0,10.0,0.0);

const float cols = 10.0;
const float rows = 10.0;
const float totalFrames = cols * rows; // 16 帧


vec3 getCamera(vec2 uv)
{
    vec2 mouse = iMouse.xy / iResolution.xy;

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


void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (fragCoord - 0.5*iResolution.xy)/iResolution.y;
    vec3 rayDir = getCamera(uv);

    // plane intersection
    float t = (0.0 - camPos.y) / rayDir.y;

    if(t < 0.0)
    {
        fragColor = vec4(0,0,0.7,0.0);
        return;
    }

    vec3 pos = camPos + t * rayDir;

    vec2 atlasSize = iChannelResolution[0].xy;
    vec2 frameSize = atlasSize / vec2(cols, rows);
    float frameIndex = floor(mod(iTime * 10.0, totalFrames)); // 10 fps 示例
    // 计算当前帧在图集中的行列位置
    float row = floor(frameIndex / cols);
    float col = mod(frameIndex, cols);
    vec2 uvStart = vec2(col / cols, 1.0 - row / rows);
    vec2 uvSize = vec2(1.0 / cols, 1.0 / rows);

    vec3 color = vec3(0.0);

    // quad size
    float rotorSize = 10.0;
    float oceanSize = 50.0;

    vec2 rotorUV = (pos.xz / rotorSize) * 0.5 + 0.5;
    vec2 oceanUV = (pos.xz / oceanSize) * 0.5 + 0.5;

    // if(abs(pos.x) > rotorSize || abs(pos.z) > rotorSize)
    // {
    //     color = vec3(0.0,0.7,0.0);
    //     color = texture2D(iChannel1, oceanUV).rgb;
    // }else{
    //     color = texture2D(iChannel0, rotorUV).rgb;
    // }
    color = texture2D(iChannel1, oceanUV).rgb;

    // 计算 alpha
    float r = length(rotorUV-vec2(0.5,0.5)); 
    float alpha = 1.0- r/length(vec2(0.5,0.5));
    alpha = pow(alpha, 0.5);

    if(abs(pos.x) < rotorSize && abs(pos.z) < rotorSize)
    {
        rotorUV = uvStart + rotorUV * uvSize;
        vec3 rotorcolor = texture2D(iChannel0, rotorUV).rgb;
        color = mix(color, rotorcolor, alpha);
    }

    fragColor = vec4(color,1.0);
}
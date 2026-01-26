
float rand(vec2 p){
    float h = dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}

// 生成随机方向
vec3 randomDir(vec2 seed) {
    float phi = rand(seed) * 6.2831853;
    float cosTheta = rand(seed + 0.1);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    return vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}

// 核心扰动函数
vec3 perturbNormal(vec2 uv, vec3 normal, float roughness) {
    // 生成随机方向
    vec3 rnd = randomDir(uv * iResolution.xy);
    
    // 简单切线空间（假设法线不垂直）
    vec3 tangent = normalize(cross(vec3(0.0, 1.0, 0.0), normal));
    if (length(tangent) < 0.001) tangent = vec3(1.0, 0.0, 0.0);
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);
    
    // 应用扰动
    return normalize(normal + tbn * rnd * roughness * 0.1);
}


// 使用梯度噪声或Perlin噪声替代白噪声
vec3 continuousRandom(vec2 uv) {
    // 使用低通滤波的噪声（如值噪声或梯度噪声）
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    
    // 双线性插值
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));
    
    //平滑步进函数（Smoothstep）的三次多项式形式
    vec2 u = f * f * (3.0 - 2.0 * f); // 平滑插值
    float value = mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
    
    // 生成连续方向
    float phi = value * 6.2831853;
    float cosTheta = rand(uv + 0.1);
    return vec3(cos(phi), sin(phi), cosTheta);
}



void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;

    vec3 rnd = continuousRandom(fragCoord*0.1); // 降低频率增加连续性
    //rnd = randomDir(fragCoord*0.1);
    fragColor=vec4( rnd, 1.0 );
}
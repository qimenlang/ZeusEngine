#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform vec2 resolution;

void main()
{
    vec2 offset = vec2(1.0 / resolution);
    vec2 offsets[9] = vec2[](
        vec2(-offset.x,  offset.y), // 左上
        vec2( 0.0f,    offset.y), // 正上
        vec2( offset.x,  offset.y), // 右上
        vec2(-offset.x,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset.x,  0.0f),   // 右
        vec2(-offset.x, -offset.y), // 左下
        vec2( 0.0f,   -offset.y), // 正下
        vec2( offset.x, -offset.y)  // 右下
    );

    //均值模糊
    float avg_blur_kernel[9] = float[](
        1.f/9, 1.f/9, 1.f/9,
        1.f/9, 1.f/9, 1.f/9,
        1.f/9, 1.f/9, 1.f/9
    );
    // 高斯模糊
    float gassin_blur_kernel[9] = float[](
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16    
    );

    //锐化 :保留内容的同时,凸出差异,和为1
    float sharp_kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    // 拉普拉斯锐化
    float laplas_sharp_kernel[9] = float[](
        0, -1,  0,
        -1,  5, -1,
        0, -1,  0
    );

    //边缘检测核，追求隐藏低频区域，和为0,在平坦区域，梯度为0（输出黑色）；在变化剧烈的边缘，梯度很大（输出亮色）
     float edge_kernel[9] = float[](
        1, 1,  1,
        1, -8, 1,
        1, 1,  1
    );
    float edge_kernel2[9] = float[](
        -2, -2, -2,
        -2,  16, -2,
        -2, -2, -2
    );
    
    //Sobel X (检测垂直边缘)
    float sobel_x_sharp_kernel[9] = float[](
        -1, 0,  1,
        -2, 0, 2,
        -1, 0,  1
    );
    // Sobel Y (检测水平边缘)
    float sobel_y_sharp_kernel[9] = float[](
        -1, -2, -1,
         0, 0, 0,
         1, 2, 1
    );

    // 浮雕滤波器
    float sculpture_kernel[9] = float[](
        -2, -1, 0,
         -1, 1, 1,
         0, 1, 2
    );

    float kernel[9] = laplas_sharp_kernel ;

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}

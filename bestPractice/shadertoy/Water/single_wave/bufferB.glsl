// shadertoy: bufferB
#iKeyboard
#iChannel0 "self"   // 读取自己的上一帧输出

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec3 cam;

    if(iFrame == 0)
        cam = vec3(0.0, 0.0, 0.0);
    else
        cam = texture2D(iChannel0, vec2(0.5)).xyz;

    float speed = 4.0;

    if(isKeyDown(Key_T)) cam.y += speed; // T
    if(isKeyDown(Key_G)) cam.y -= speed; // G
    if(isKeyDown(Key_F)) cam.x -= speed; // F
    if(isKeyDown(Key_H)) cam.x += speed; // H

    fragColor = vec4(cam, 1.0);
}
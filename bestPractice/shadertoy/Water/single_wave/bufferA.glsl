// shadertoy: bufferA
#iKeyboard
#iChannel0 "self"   // 读取自己的上一帧输出

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec3 cam;

    if(iFrame == 0)
        cam = vec3(0.0, 10.0, 0.0);
    else
         cam = texture2D(iChannel0, vec2(0.5)).xyz;

    float speed = 0.2;

    if(isKeyDown(Key_W)) cam.z -= speed; // W
    if(isKeyDown(Key_S)) cam.z += speed; // S
    if(isKeyDown(Key_A)) cam.x -= speed; // A
    if(isKeyDown(Key_D)) cam.x += speed; // D
    if(isKeyDown(Key_C)) cam.y -= speed; // C
    if(isKeyDown(Key_E)) cam.y += speed; // E

    fragColor = vec4(cam, 1.0);
}
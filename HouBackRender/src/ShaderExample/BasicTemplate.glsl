//this is from shadertoy default start shader!\n
//see in https://www.shadertoy.com/new\n
#version 400 core
// do not change the below variable name
out vec4 FragColor;
in vec2 uv;
uniform float inTime;
uniform vec2 inMousePos;

void main()
{
    float iTime = inTime;
    vec2 mousePos = inMousePos;
    vec2 inuv = vec2(uv.x, 1.0 - uv.y);
    //invu.x *= 16.0f/9.0f;
    vec3 col = 0.5 + 0.5 * cos(iTime + inuv.xyx + vec3(0,2,4));
    FragColor = vec4(col,1.f);
}

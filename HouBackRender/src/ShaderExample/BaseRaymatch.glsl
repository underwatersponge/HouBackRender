//this is from shadertoy default start shader!\n
//see in https://www.shadertoy.com/new\n

#version 400 core

#define MAX_DIST 100.0f
#define MAX_ITE 100.0f
#define SUR_LEN 0.0001
#define PI 3.1415

mat2 Rot(float angle)
{
	float s = sin(angle);
	float c = cos(angle);
	return mat2(c, -s, s, c);
}

float Sphere(vec3 pos, vec3 cen, float radius)
{
	float dist = length(pos - cen);
	return dist - radius;
}

float Sphere(vec3 pos, float radius)
{
	float dist = length(pos) - radius;
	return dist;
}

float Box(vec3 pos, vec3 size)
{
	pos = abs(pos) - size;
	return length(max(pos,0.f)) + min(max(pos.x, max(pos.y, pos.z)),0.f);
}

float Eliptical(vec3 pos, vec3 scale)
{
	pos/= scale;
	float dist = (length(pos) - 1.0) * min(min(scale.x, scale.y), scale.z);
	return dist;
}

float GetDist(vec3 pos)
{
	float box = Box(pos, vec3(1.0));
	float sphere = Sphere(pos,0.5f);

	float dist = box;
	//dist = sphere;
	return dist;
}

float RayMarch(vec3 ro, vec3 rd)
{
	float stepLen = 0.0;
	float weak = 0.0;
	vec3 pos = vec3(0.0);
	
	for(float i=0.0; i<MAX_ITE; i++)
	{
		pos = ro + rd * weak;
		stepLen = GetDist(pos);
		weak += stepLen;
		if(weak > MAX_DIST || stepLen < SUR_LEN)
			break;
	}
	return weak;
}

vec3 GetNormal(vec3 pos)
{
	float dist = GetDist(pos);
	vec2 e = vec2(0.001,0.0);
	vec3 normal = dist - vec3(
		GetDist(pos - vec3(e.xyy)),
		GetDist(pos - vec3(e.yxy)),
		GetDist(pos - vec3(e.yyx))
	);
	
	return normalize(normal);
}

float Light(vec3 pos, vec3 lightPos){
    vec3 normal = GetNormal(pos);
    vec3 lightDir = normalize(lightPos - pos);
    float diff = dot(lightDir, normal) * 0.5 + 0.5;
    return diff;
}

float Light(vec3 ro, vec3 rd, float dist, vec3 lightPos){
    if(dist < MAX_DIST){
        vec3 pos = ro + dist * rd;
        vec3 normal = GetNormal(pos);
        vec3 lightDir = normalize(lightPos - pos);
        float diff = dot(lightDir, normal) * 0.5 + 0.5;
        // diff = max(dot(lightDir, normal), 0.0);
    return diff;
    }
    return 0.0;
}

vec3 GetRayDir(vec3 ro, vec3 lookAt, vec2 uv, float zoom)
{
	vec3 front = normalize(lookAt - ro);
	vec3 right = normalize(cross(vec3(0.f,1.f,0.f), front));
	vec3 up = cross(front, right);

	vec3 cen = ro + front * zoom;
	vec3 i = cen + uv.x * right + uv.y * up;
	vec3 rd = normalize(i - ro);
	
	return rd;
}


out vec4 FragColor;
in vec2 uv;
uniform float inTime;
uniform vec2 inMousePos;

void main()
{
    float iTime = inTime;
    vec2 mouse = inMousePos/1000.f;
    vec2 inuv = vec2(uv.x, 1.f - uv.y);
	inuv -= 0.5f;	
	inuv.x /= 9.0/16.0f;
	
	vec3 col = vec3(0.f);

	vec3 ro = vec3(0.f,1.f,-3.f);
	ro.yz *= Rot(-mouse.y * PI / 1.0);
	ro.xz *= Rot(-mouse.x * PI / 2.0);

	vec3 lookAt = vec3(0.f,0.f,0.f);
	float zoom = 1.0f;
	vec3 rd = GetRayDir(ro, lookAt, inuv, zoom);

	float dist = RayMarch(ro, rd);
	float diff = 0.0;
	if(dist < MAX_DIST)
	{
		vec3 pos = ro + rd * dist;
		vec3 normal = GetNormal(pos);
		diff = Light(pos, ro);
	}
	col += diff;
	
    FragColor = vec4(col,1.f);
	
}

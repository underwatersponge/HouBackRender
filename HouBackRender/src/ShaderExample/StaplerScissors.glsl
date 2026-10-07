//this is from shadertoy default start shader!
//see in https://www.shadertoy.com/new
#version 400 core

#define MAX_DIST 100.0
#define MAX_ITE 300.0
#define SUR_LEN 0.001
#define PI 3.1415

mat2 Rot(float angle){
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}

vec2 Rand(vec2 pos){
    vec3 m = fract(pos.xyx * vec3(244.5, 37130.3, 2464.2));
    m += dot(m, m + vec3(32.4));
    return fract(vec2(m.x * m.y, m.y * m.z));
}

float smin(float a, float b, float k){
    float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    return mix(b, a, h) - k * h * (1.0 - h);
}

float Sphere(vec3 pos, float radius){
    float dist = length(pos) - radius;
    return dist;
}

float Box(vec3 pos, vec3 size){
    pos = abs(pos) - size;
    return length(max(pos, 0.0)) + min(max(pos.x, max(pos.y, pos.z)), 0.0);
}

float Box(vec2 pos, vec2 size){
    pos = abs(pos) - size;
    return(length(max(pos, 0.0)) + min(max(pos.x, pos.y), 0.0));
    // return 0.0;
}

float Elliptical(vec3 pos, vec3 scale, float r){
    pos /= scale;
    float dist = (length(pos) - r ) * min(min(scale.x, scale.y), scale.z);
    return dist;
}

float Tours(vec3 pos, float r1, float r2){
    float d = length(vec2(pos.xy)) - r1;
    float dist = length(vec2(d, pos.z)) - r2;
    return dist;
}

float Line(vec3 pos, vec3 a, vec3 b){
    vec3 ab = b - a;
    vec3 ap = pos - a;
    float proj = dot(ap, ab) / dot(ab, ab);
    proj = clamp(proj, 0.0, 1.0);
    vec3 c = a + proj * ab;
    float dist = length(pos - c);

    return dist;
}


float Stapler(vec3 pos){
    pos.y -= 1.37;
    // bottom shape
    // 1
    vec3 baseSize = vec3(1.0, 0.1, 0.25);
    float box1 = Box(pos, baseSize);
    float bias = 0.02;
    float xOffset = 0.35;
    float box2 = Box(pos - vec3(-xOffset, -0.03, 0.0), vec3(baseSize.x - bias - xOffset, 0.12, baseSize.z - bias));
    float smoalBox1 = Box(vec3(pos.x, pos.y, abs(pos.z)) - vec3(0.4, 0.0, 0.1), vec3(0.08, 0.13, 0.02));
    float smoalBox2 = Box(vec3(pos.x, pos.y, abs(pos.z)) - vec3(0.9, 0.0, 0.15), vec3(0.08, 0.13, 0.02));
    float smoalBox = min(smoalBox1, smoalBox2);
    float bottom = max(box1, -box2);
    // 2
    vec3 offset = vec3(0.6, 0.2, 0.0);
    float rate = (pos.y -offset.y);
    float sig = max(sign(pos.y - offset.y), 0.0);
    offset.y += (0.5 - pos.x) * 0.15 * sig;
    sig = abs(min(sign(pos.x - offset.x), 0.0));
    offset.x += 0.76 * rate * sig;
    vec3 BRsize = vec3(baseSize - vec3(0.6, 0.0, 0.0)); 
    float box3 = Box(pos - offset, BRsize);
    // 3
    float bigBox = Box(pos, baseSize + vec3(0.0, 0.3, 0.0));

    bottom = max(bottom, -smoalBox);
    float bottom1 = min(bottom, box3);
    bottom = smin(bottom, box3, 0.2);
    bottom = max(bottom, bigBox);

    // middle
    vec3 middlePos = pos - vec3(0.03, 0.3, 0.0);
    vec3 middleSize = baseSize - vec3(0.1, 0.0, 0.0737);
    // middlePos.xy *= Rot(PI * 0.033);
    // 1
    float middleBox1 = Box(middlePos, middleSize);
    // 2
    middlePos -= vec3(0.0, 0.1, 0.0);
    middleSize -= vec3(0.0, 0.0, 0.03);
    float middleBox2 = Box(middlePos, middleSize);
    // 3
    middlePos -= vec3(-0.05, 0.1, 0.0);
    middlePos.xy *= Rot(PI * 0.06);
    middleSize -= vec3(0.07, 0.02, 0.02);
    float middleBox3 = Box(middlePos, middleSize);
    // 4
    middlePos -= vec3(-0.0, -0.05, 0.0);
    middleSize -= vec3(0.01, 0.02, 0.0);
    float middleBox4 = Box(middlePos, middleSize);
    float middletop = max(middleBox3, -middleBox4);

    float middle = max(middleBox1, -middleBox2);
    middle = min(middle, middletop);
    // top
    // 1
    vec3 newPos = pos + vec3(-0.05, -0.5, 0.);
    newPos.xy *= Rot(PI * 0.1);
    float topSign = newPos.x > 0.0002 ? 1.0 : -1.0;
    newPos.x += sin((newPos.z - 1.2) * 1.2) * 0.37 * topSign;
    newPos.x += 0.32 * topSign;
    newPos.y += sin(newPos.x * 1.73 - 1.73) * 0.073;
    vec3 size = baseSize -vec3(0.1, -0.03, 0.0);
    float topbox = Box(newPos, size) - 0.02;
    // 2
    vec3 newPos1 = newPos;
    newPos1 -= vec3(0.7, -0.1, -0.);
    vec3 topOffset = vec3(0.3, 0.0, -0.1);
    float topRate = newPos1.y - topOffset.y;
    float topOffsetX = 1.1 * topRate;
    topOffset.x += topOffsetX;
    vec3 topSize1 = size - topOffset;
    float topBox1 = Box(newPos1, topSize1);

    vec3 newPos2 = newPos;
    newPos2 -= vec3(0.49, 0.0, 0.0);
    vec3 topSize2 = size - vec3(0.48, 0.0, 0.0);
    float topBox2 = Box(newPos2, topSize2);

    // 3
    newPos.y += 0.1;
    size -= vec3(0.01, 0., 0.02);
    float topbox1 = Box(newPos, size);
    topbox = max(topbox, -topBox1);
    topbox = min(topbox, topBox2);
    float top = max(topbox, -topbox1);

    float shape = min(top, bottom);
    shape = min(shape, middle);
    // shape = topBox2;//test
    float dist = shape;
    return dist;
}

float ScissorsHalf(vec3 pos){
    pos.x -= 0.437;
    // bottomHandlebar
    // 1
    // float line = Line(pos, vec3(0.5, 0.0, 0.0), vec3(-0.5, .0, 0.0)) - 0.05;
    float tours = Tours(pos, 0.5, 0.05);
    tours = max(tours, pos.y);
    float bigLine = Line(pos, vec3(0.5, 0.0, 0.0), vec3(-0.5, .0, 0.0)) - 0.067;
    float bhBox = Box(pos + vec3(0.0, 0.25, 0.0), vec3(0.73, 0.3, 0.05));
    float bottomH = smin(tours, bigLine, 0.1);
    bottomH = max(bottomH, bhBox);

    //bottom knife
    // 1
    vec3 newPos = pos + vec3(0.2, -0.037, 0.0);
    float bkBox1 = Box(newPos, vec3(0.4, 0.073, 0.02));// - 0.02;
    float bkBox3 = Box(newPos + vec3(0.29, 0.1, 0.0), vec3(0.04, 0.073, 0.02));
    vec3 newPos1 = newPos += vec3(0.5, -0.03, 0.0);
    newPos1.xy *= Rot(PI * 0.073);
    float bkBox2 = Box(newPos1, vec3(0.15, 0.073, 0.02));
    bkBox2 = smin(bkBox2, bkBox3, 0.1);
    float e = Elliptical(newPos - vec3(0.3, 0.12, 0.0), vec3(2.5, 1.0, 1.0), 0.1);
    // 2
    vec3 newPos2 = newPos + vec3(0.73, -0.1, 0.0);
    newPos2.xy *= Rot(PI * 0.037);
    float bkBox4 = Box(newPos2, vec3(0.6, 0.073, 0.02));
    newPos2.xy *= Rot(PI * -0.03);
    float bkBox5 = Box(newPos2 - vec3(0.0, 0.1, 0.0), vec3(0.6, 0.05, 0.05));
    bkBox4 = max(bkBox4, -bkBox5);//
    newPos2.xy *= Rot(PI * -0.07);
    float bkBox6 = Box(newPos2 - vec3(-0.37, 0.13, 0.0), vec3(0.37, 0.064, 0.05));
    bkBox4 = max(bkBox4, -bkBox6);//

    float bottomK = min(bkBox1, bkBox2);
    bottomK = max(bottomK, -e) - 0.0073;
    bottomK = min(bottomK, bkBox4);

    float bottom = min(bottomH, bottomK);
    // bottom = bottomK;
    float shape = bottom;
    return shape;
}

float Scissors(vec3 pos){
    pos.xy *= Rot(PI * 0.33);
    vec3 pos1 = pos;
    pos1.xy *= Rot(PI * 0.073);
    float shapeBottom = ScissorsHalf(pos1);

    pos -= vec3(0.0, 0.37, 0.0);
    pos.xy *= Rot(PI * -0.15);
    pos.y *= -1.0;
    float shapeTop = ScissorsHalf(pos);
    float shape = min(shapeBottom, shapeTop);
    
    return shape;
}


float Ruler(vec3 pos){
    pos.xy *= Rot(PI * 0.12);
    vec3 size = vec3(1.37, 0.173, 0.0173);
    pos -= vec3(-0.2, -1.37, 0.2);
    float rate = (pos.y + size.y) / 0.346;
    float signR = sign(pos.x) > 0.0? 1.0 : 0.0;
    pos.x += rate * 0.37;// * signR;
    float box = Box(pos, size);
    return box;
}

float GetDist(vec3 pos){

    float stapler = Stapler(pos);
    float scissors = Scissors(pos);
    float ruler = Ruler(pos);

    float dist = min(stapler, scissors);
    dist = min(dist, ruler);
    return dist;
}

float RayMarch(vec3 ro, vec3 rd){
    float stepLen = 0.0;
    float weak = 0.0;
    vec3 pos = vec3(0.0);
    for(float i = 0.0; i<MAX_ITE; i++){
        pos = ro + weak * rd;
        stepLen = GetDist(pos);
        weak += stepLen;
        if(weak > MAX_DIST || stepLen < SUR_LEN)
            break;
    }
    return weak;
}

vec3 GetNormal(vec3 pos){
    float dist = GetDist(pos);
    vec2 e = vec2(0.001, 0.0);
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

vec3 GetRayDir(vec3 ro, vec3 lookAt, vec2 uv, float zoom){
    vec3 front = normalize(lookAt - ro);
    vec3 right = cross(vec3(0.0, 1.0, 0.0), front);
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
    vec2 mousePos = inMousePos;
	vec2 inUv = vec2(uv.x - 0.5, 0.5-uv.y);
    inUv.x *= 16.0f/9.0f;
	vec3 col = vec3(0.f);
	
	vec3  ro = vec3(3.f,1.f,-5.f);
	ro.yz *= Rot(-mousePos.y/600.f * PI /1.0);
	ro.xz *= Rot(-mousePos.x/600.f * PI /2.0);

	vec3 lookAt = vec3(0.f,0.f,0.f);
	float zoom = 1.0;
	vec3 rd = GetRayDir(ro, lookAt, inUv, zoom);
	
	float dist = RayMarch(ro, rd);	
	float diff = 0.f;
	if(dist < MAX_DIST){
		vec3 pos = ro + rd * dist;
		vec3 normal = GetNormal(pos);
		diff = Light(pos, vec3(1.0,0.2,-0.3));
	}
	col += diff;
	FragColor = vec4(col,1.f);
}

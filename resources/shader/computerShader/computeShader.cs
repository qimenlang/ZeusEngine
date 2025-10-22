#version 460 core

// layout 在前还是数据结构在前
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
    vec3 tangent;
    vec3 bitangent;
    vec4 bone_ids;
    vec4 weights;
};

struct Ray {
    vec3 origin;
    vec3 direction;
    float t_min;
    float t_max;
};

struct HitRecord {
    float t;
    vec3 position;
    vec3 normal;
    vec2 uv;
    int material_id;
    bool front_face;
};

struct Camera {
	vec3 origin;
	vec3 direction;
	vec3 up;
	float v_fov;
	float aspect_ratio;
};

// 该layout直接定义了每个工作组的xyz维度的invocation数量
layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout(std430, binding = 1) buffer VertexBuffer {
	Vertex vertices[];
};
layout(std430, binding = 2) buffer DebugOutput {
    int debug_data[];
};

layout (location = 0) uniform float t;                 /** Time */

// ----------------------------------------------------------------------------
//
// functions
//
// ----------------------------------------------------------------------------


vec4 red = vec4(1.0, 0.0, 0.0, 1.0); // NO
vec4 green = vec4(0.0, 1.0, 0.0, 1.0); // YES
vec4 blue = vec4(0.0, 0.0, 1.0, 1.0); //
vec4 black = vec4(0.0, 0.0, 0.0, 1.0); // 
vec4 white = vec4(1.0,1.0,1.0, 1.0); // 

// Moller-Trumbore 光线求交 
bool intersectTest(Ray ray,vec3 v0, vec3 v1, vec3 v2) {
	// intersect test
	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;
	vec3 h = cross(ray.direction, edge2);
	float a = dot(edge1, h);

	if (abs(a) < 0.0001) return false; // This ray is parallel to this triangle.
	
	vec2 uv;
	float f = 1.0 / a;
	vec3 s = ray.origin - v0;
	uv.x = f * dot(s, h);
	if (uv.x < 0.0 || uv.x > 1.0) return false;
	
	vec3 q = cross(s, edge1);
	uv.y = f * dot(ray.direction, q);
	if (uv.y < 0.0 || uv.x + uv.y > 1.0) return false;
	
	float t = f * dot(edge2, q);
	return t > 0.0001;
}

bool intersectTriangle(Ray ray) {
	for(int i=0;i<vertices.length();i+=3){
		vec3 v0 = vertices[i].position;
		vec3 v1 = vertices[i+1].position;
		vec3 v2 = vertices[i+2].position;
		if(intersectTest(ray,v0,v1,v2))
			return true;
	}
	return false;
}


Ray generateRay(Camera camera, ivec2 texelCoord) {
	Ray ray;
	ivec2 screenSize = ivec2(gl_NumWorkGroups.x * gl_WorkGroupSize.x, gl_NumWorkGroups.y * gl_WorkGroupSize.y);
	ray.origin = camera.origin;

	vec3 offset;
	offset.xy = vec2((float(texelCoord.x)+0.5)/screenSize.x,(float(texelCoord.y)+0.5)/screenSize.y) * 2.0 - 1.0;
	
	offset.z = -1.0f;
	ray.direction = normalize(offset);
	return ray;
}

float abBox( in vec2 p, in vec2 size )
{
    vec2 d = abs(p) - size;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main() {
	// screenSize
	ivec2 screenSize = ivec2(gl_NumWorkGroups.x * gl_WorkGroupSize.x, gl_NumWorkGroups.y * gl_WorkGroupSize.y);
	//gl_GlobalInvocationID :当前线程在全局所有线程中的唯一ID
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

	// camera config 
	Camera camera;
	camera.origin = vec3(0.0, 0.0, 1.0);
	camera.direction = vec3(0.0, 0.0, -1.0);
	camera.up = vec3(0.0, 1.0, 0.0);
	camera.v_fov = 45.0;
	camera.aspect_ratio = screenSize.x / float(screenSize.y);
	
	// debug info 
	debug_data[0] = vertices.length();
	debug_data[1] = screenSize.x;
	debug_data[2] = screenSize.y;

	vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
	float speed = 100;
	// the width of the texture
	float width = 1000;

	// gl_NumWorkGroups.xyz ：在cpu端通过glDispatchCompute(x,y,z)分派的工作组三个维度的数量
	// gl_WorkGroupSize.xyz : 对应layout (local_size_x = ...) in;中设置的工作组布局

	// value.x = mod(float(texelCoord.x) + t * speed, width) / (gl_NumWorkGroups.x * gl_WorkGroupSize.x);
	// value.y = float(texelCoord.y)/(gl_NumWorkGroups.y*gl_WorkGroupSize.y);

	value.x = float(texelCoord.x)/(screenSize.x);
	value.y = float(texelCoord.y)/(screenSize.y);
	// value.x = 0.0;
	// value.y = 0.0;

	// sphere sdf
	// if(pow(value.x-0.0,2)+pow(value.y-0.0,2)<0.1 ){
	// 	imageStore(imgOutput, texelCoord, vec4(1.0,1.0,1.0,1.0));
	// 	return;
	// }

	// box sdf
	// vec2 boxSize = vec2(0.1, 0.1);
	// vec2 boxCenter = vec2(0.5, 0.5);
	// if(abBox(value.xy - boxCenter,boxSize)<0.001){
	// 	imageStore(imgOutput, texelCoord, vec4(1.0,0,0,1.0));
	// 	return;
	// }


	// for (int i=0;i<vertices.length();i++){
	// 	vec3 v0 = vertices[i].position;
	// 	// vec3 v0 = vertices[i*3].position;
    //     // vec3 v1 = vertices[i*3+1].position;
    //     // vec3 v2 = vertices[i*3+2].position;
	// 	// imageStore(imgOutput, texelCoord, vec4(1.0,1.0,1.0,1.0));

	// 	if(abs(value.x-v0.x)<0.01 && abs(value.y-v0.y) <0.01){
	// 		imageStore(imgOutput, texelCoord, green);
	// 		return;
	// 	}
	// }

	Ray ray =  generateRay(camera, texelCoord);
	// imageStore(imgOutput, texelCoord, vec4(ray.direction.xy,0,1.0));

	if(intersectTriangle(ray))
		imageStore(imgOutput, texelCoord, green);
	else
		imageStore(imgOutput, texelCoord, black);
	
	// if(intersect(ray)){
	// 	// imageStore(imgOutput, texelCoord, green);
	// 	return;
	// }

	// imageStore(imgOutput, texelCoord, black);
}
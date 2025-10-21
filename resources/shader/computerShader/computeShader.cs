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

bool intersect() {
	return false;
}

bool intersect(Ray ray, out HitRecord rec) {
	return false;
}




float abBox( in vec2 p, in vec2 size )
{
    vec2 d = abs(p) - size;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}
void main() {
	debug_data[0] = vertices.length();

	vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
	//gl_GlobalInvocationID :当前线程在全局所有线程中的唯一ID
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	float speed = 100;
	// the width of the texture
	float width = 1000;

	// gl_NumWorkGroups.xyz ：在cpu端通过glDispatchCompute(x,y,z)分派的工作组三个维度的数量
	// gl_WorkGroupSize.xyz : 对应layout (local_size_x = ...) in;中设置的工作组布局

	// value.x = mod(float(texelCoord.x) + t * speed, width) / (gl_NumWorkGroups.x * gl_WorkGroupSize.x);
	// value.y = float(texelCoord.y)/(gl_NumWorkGroups.y*gl_WorkGroupSize.y);

	value.x = float(texelCoord.x)/(gl_NumWorkGroups.x * gl_WorkGroupSize.x);
	value.y = float(texelCoord.y)/(gl_NumWorkGroups.y * gl_WorkGroupSize.y);
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

	vec4 red = vec4(1.0, 0.0, 0.0, 1.0); // NO
	vec4 green = vec4(0.0, 1.0, 0.0, 1.0); // YES
	vec4 blue = vec4(0.0, 0.0, 1.0, 1.0); //
	vec4 black = vec4(0.0, 0.0, 0.0, 1.0); // 


	for (int i=0;i<vertices.length();i++){
		vec3 v0 = vertices[i].position;
		// vec3 v0 = vertices[i*3].position;
        // vec3 v1 = vertices[i*3+1].position;
        // vec3 v2 = vertices[i*3+2].position;
		// imageStore(imgOutput, texelCoord, vec4(1.0,1.0,1.0,1.0));

		if(abs(value.x-v0.x)<0.01 && abs(value.y-v0.y) <0.01){
			imageStore(imgOutput, texelCoord, green);
			return;
		}
	}

	imageStore(imgOutput, texelCoord, black);
}
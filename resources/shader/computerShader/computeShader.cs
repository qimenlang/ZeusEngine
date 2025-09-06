#version 430 core

// 该layout直接定义了每个工作组的xyz维度的invocation数量
layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout(rgba32f, binding = 0) uniform image2D imgOutput;

layout (location = 0) uniform float t;                 /** Time */

// ----------------------------------------------------------------------------
//
// functions
//
// ----------------------------------------------------------------------------

void main() {
	vec4 value = vec4(0.0, 0.0, 0.0, 1.0);
	//gl_GlobalInvocationID :当前线程在全局所有线程中的唯一ID
	ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
	float speed = 100;
	// the width of the texture
	float width = 1000;

	// gl_NumWorkGroups.xyz ：在cpu端通过glDispatchCompute(x,y,z)分派的工作组三个维度的数量
	// gl_WorkGroupSize.xyz : 对应layout (local_size_x = ...) in;中设置的工作组布局

	value.x = mod(float(texelCoord.x) + t * speed, width) / (gl_NumWorkGroups.x * gl_WorkGroupSize.x);
	value.y = float(texelCoord.y)/(gl_NumWorkGroups.y*gl_WorkGroupSize.y);
	imageStore(imgOutput, texelCoord, value);
}
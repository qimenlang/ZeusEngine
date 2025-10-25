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
	vec3 world_position;
	vec3 world_front;
	vec3 world_right;
	vec3 world_up;
	float v_fov;
	float aspect_ratio;
};
uniform Camera camera;


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
layout(std430, binding = 2) buffer IndexBuffer {
	uint indices[];
};
layout(std430, binding = 3) buffer DebugOutput {
    float debug_data[];
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
	for(int i=0;i<indices.length();i+=3){
		uint idx0 = indices[i];
		uint idx1 = indices[i+1];
		uint idx2 = indices[i+2];
		vec3 v0 = vertices[idx0].position;
		vec3 v1 = vertices[idx1].position;
		vec3 v2 = vertices[idx2].position;
		if(intersectTest(ray,v0,v1,v2))
			return true;
	}
	return false;
}

// 光线-AABB 相交检测,非常高效
bool intersectRayAABB(vec3 rayOrigin, vec3 rayDirection, vec3 aabbMin, vec3 aabbMax, out float tEnter, out float tExit) {
    // 分别处理 x, y, z 轴
	for (int i = 0; i < 3; ++i) {  
		// t0,t1分别为射线到达最小最大点的时间
        float t0 = (aabbMin[i] - rayOrigin[i]) / rayDirection[i];
        float t1 = (aabbMax[i] - rayOrigin[i]) / rayDirection[i];
        // 确保t0是近交点，t1是远交点，注意上面除以了direction在轴上的分量
        // if (invD < 0.0f) t0, t1);
		float tnear = min(t0,t1);
		float tfar = max(t0,t1);
        
		// 取三个轴近交点的最大值，远交点的最小值
        tEnter = max(tnear, tEnter);
        tExit = min(tfar, tExit);
        // 远交点比近交点还近，说明没有有效相交
        if (tExit <= tEnter) return false;
    }
    return true;
}


Ray generateRay(Camera camera, ivec2 texelCoord) {
	ivec2 screenSize = ivec2(gl_NumWorkGroups.x * gl_WorkGroupSize.x, gl_NumWorkGroups.y * gl_WorkGroupSize.y);
	vec2 uv;
	uv.xy = vec2((float(texelCoord.x)+0.5)/screenSize.x,(float(texelCoord.y)+0.5)/screenSize.y);
	uv.xy = uv.xy * 2.0 - 1.0;// 转换到[-1,1]

	float aspect_ratio = camera.aspect_ratio;
	float tan_half_fov = tan(radians(camera.v_fov) * 0.5);
	// 右手坐标系中，相机默认朝向 -Z 轴，外部传入的camera world_front已经默认-z了
	// 这里是相机的局部坐标系，z轴fornt是正方向，不需要再次-z
	vec3 camera_space_dir = normalize(vec3(uv.x * aspect_ratio * tan_half_fov,
		 uv.y * tan_half_fov, 1.0));
	
	// 转换到世界坐标系
    mat3 camera_to_world = mat3(camera.world_right, camera.world_up, camera.world_front);
	Ray ray;
	ray.origin = camera.world_position;
	ray.direction = normalize(camera_to_world*camera_space_dir);
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
	if (texelCoord.x >= screenSize.x || texelCoord.y >= screenSize.y) 
        return;

	// camera config 
	
	// camera.v_fov = 45.0;
	// camera.aspect_ratio = screenSize.x / float(screenSize.y);
	
	// debug info 
	debug_data[0] = float(vertices.length());
	debug_data[1] = float(screenSize.x);
	debug_data[2] = float(screenSize.y);
	debug_data[3] = float(camera.world_position.x);
	debug_data[4] = float(camera.world_front.x);

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
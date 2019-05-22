#version 330 core

// Camera properties
layout (std140) uniform camera
{
	// View -> Screen transformation matrix
	mat4 projection;
	// World -> View coordinate transformation
	mat4 view;
	// Premultiplied projection * view (for efficiency)
	mat4 pvm;
	// Orthographic projection (not used in this assignment)
	mat4 ortho;
	// Camera world space position
	vec4 position;
};

// Model -> World transformation matrix
uniform mat4 model=mat4(1.0f);

// Available input 
// Object space position
layout (location=0) in vec4 pos;
// Object space normal
layout (location=2) in vec4 normal;
// Vertex texture coordinate
layout (location=3) in vec4 uv;
uniform float ratio;

// Values to pass to the fragment shader (interpolated)
// World space vertex position
out vec3 vtx_position;
// World space vertex normal
out vec3 vtx_normal;

void main()												
{
	// Taper towards the top
	float newRatio = pos.y * ratio + (1. - pos.y);
	vec4 newPos = vec4(pos.x * newRatio, pos.y, pos.z * newRatio, 1.);

	gl_Position=pvm*model*newPos;
	vtx_position=(model*newPos).xyz;
	vtx_normal=normalize((transpose(inverse(model)) * normal).xyz);
}

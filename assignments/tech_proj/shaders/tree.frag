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

// Light properties
struct light
{
	ivec4 att;
	vec4 pos;
	vec4 dir;
	vec4 amb;
	vec4 dif;
	vec4 spec;
	vec4 atten;
	vec4 r;
};

layout (std140) uniform lights
{
	vec4 amb;	// ambient color
	ivec4 lt_att;	// lt_att[0]: number of lights
	light lt[2];	// Values for each light
};

// Input attributes, written in the vertex shader and interpolated
in vec3 vtx_normal;
in vec3 vtx_position;

// Output attributes, final color of the fragment
out vec4 frag_color;

vec3 phong(int i, vec4 baseColor, vec3 n, vec3 v, float shininess, float specStrength) {
	vec3 lightDir =  normalize(-lt[i].dir.xyz);
	vec3 lightCol = lt[i].dif.rgb;
	vec3 ambient_color = amb.rgb;

	vec3 phong_lighting = vec3(0);

	vec3 r = -lightDir + (2 * dot(lightDir, n) * n);

	phong_lighting += ambient_color;
	phong_lighting += baseColor.rgb * max(0, dot(lightDir, n)) * lightCol;
	phong_lighting += lightCol * pow(max(0, dot(v, r)), shininess) * specStrength;

	return phong_lighting;
}

vec3 lighting(vec4 baseColor, vec3 n, vec3 v, float shininess, float specStrength) {
	vec3 color = vec3(0);
	for(int i=0;i<lt_att[0];i++) {
		color+=phong(i,baseColor,n,v, shininess, specStrength);
	}
	return color;
}

void main()
{
	vec4 baseColor = vec4(.326,.208,.039,1.); 
	vec3 norm = normalize(vtx_normal.xyz);
	vec3 vertex_to_eye = normalize(position.xyz - vtx_position);
	float shininess = 80;
	float specStrength = 0.1;
	vec3 color = lighting(baseColor, norm, vertex_to_eye, shininess, specStrength);
	frag_color=vec4(color.rgb,1.);
}

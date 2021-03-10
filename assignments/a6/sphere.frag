/*This is your fragment shader for texture and normal mapping*/

#version 330 core

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*uniform variables*/
uniform float iTime;					////time
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal

in vec3 vtx_pos;

out vec4 frag_color;

void main()							
{
	// calculate uv
	float pi = 3.1415926536;
	float theta = acos(vtx_pos.y / 10.); // divide by the radius
	float phi = atan(vtx_pos.x, vtx_pos.z);
	float u = phi / (2. * pi);
	float v = theta / pi;
	vec2 uv = vec2(u, v);

	// get color and normal from texture
	vec4 color = texture(tex_albedo, uv);
	vec3 normal = texture(tex_normal, uv).rgb;
	normal = normalize(2. * normal - 1.);
	frag_color = vec4(color);
}	
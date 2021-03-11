/*This is your fragment shader for texture and normal mapping*/

#version 330 core
#define newsin(x) (sin(2.0 * x) * 0.5 + 0.5)

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
uniform float iResolution;					////time
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal

in vec3 vtx_pos;

out vec4 frag_color;

float hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	// Your implementation starts here
	//Source : https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner/
	float m = dot(v, vec2(12.9898, 78.233));
	rand =  fract(vec2(sin(m),cos(m))* 43758.5453) * 2. - 1.;

	vec2 rotateRand = vec2(0.);
	rotateRand.x = cos(iTime * 0.01) * rand.x - sin(iTime  * 0.01) * rand.y;
	rotateRand.y = sin(iTime * 0.01) * rand.x - cos(iTime * 0.01) * rand.y;

	return fract(rotateRand.x);
	// return rotateRand;
	
	// Your implementation ends here
}

void stars(inout vec4 color, vec2 uv) {
	// randomizes stars
    float t = sin(iTime * 5.0 * hash2(-uv)) * 0.8 + 1.0;
	// interpolation
    color += smoothstep(0.98, 1.0, hash2(uv)) * t;
}

void auroras(inout vec4 color, vec2 uv) {
    const vec4 green = vec4(0.0, 1.2, 0.5, 1.0);
    const vec4 navy = vec4(0.0, 0.4, 0.6, 1.0);
	const vec4 purple = vec4(0.7, 0.0, 0.5, 1.0);
	float changeColor = abs(sin(iTime));
    
    float t = newsin(-iTime + uv.x * 100.0) * 0.075 + newsin(iTime + uv.x * distance(uv.x, 0.5) * 100.0) * 0.1 - 0.5;
    t = 1.0 - smoothstep(uv.y - 4.0, uv.y * 2.0, t);

    vec4 first_color = mix(purple, green, changeColor);
	vec4 final_color = mix (first_color, navy, clamp(1.0 - uv.y * t, 0.0, 1.0));
    //vec4 final_color = mix(green, navy, clamp(1.0 - uv.y * t, 0.0, 1.0));
    final_color += final_color * final_color;
    color += final_color * t * (t + 0.5) * 0.75;
}

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

	vec4 color = vec4(0.0);

	// aurora and stars
 	stars(color, uv);
	auroras(color, uv);

	vec3 normal = texture(tex_normal, uv).rgb;
	normal = normalize(2. * normal - 1.);
	frag_color = vec4(color);
}	
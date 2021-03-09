/*This is your first vertex shader!*/

#version 330 core

#define PI 3.14159265

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

///////////// Part 1a /////////////////////
/* Create a function that takes in an xy coordinate and returns a 'random' 2d vector. (There is no right answer)
   Feel free to find a hash function online. Use the commented function to check your result */
vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	// Your implementation starts here

	// Source : https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner/
	float m = dot(v, vec2(12.9898, 78.233));
	rand =  fract(vec2(sin(m),cos(m))* 43758.5453) * 2. - 1.;

	vec2 rotateRand = vec2(0.);
	rotateRand.x = cos(iTime) * rand.x - sin(iTime) * rand.y;
	rotateRand.y = sin(iTime) * rand.x - cos(iTime) * rand.y;

	return rotateRand;
	
	// Your implementation ends here

}

///////////// Part 1b /////////////////////
/*  Using i, f, and m, compute the perlin noise at point v */
float perlin_noise(vec2 v) 
{
	float noise = 0;
	// Your implementation starts here
	// calculate cell index i 
	float i = floor(v.x);
	float j = floor(v.y);
	vec2 ij = vec2(i, j);

	// 4 points
	vec2 p1 = vec2(i, j); // top left
	vec2 p2 = vec2(i + 1, j);  // top right
	vec2 p3 = vec2(i, j + 1); // bottom left
	vec2 p4 = vec2(i + 1, j + 1); // bottom right

	// calculate the fraction fr 
	float fx = fract(v.x);
	float fy = fract(v.y);
	vec2 fr = vec2(fx, fy);

	// smoothstep
	vec2 s = fr * fr * (3.0 - 2.0 * fr);
	// s.x *= 0.5 * sin(iTime + 6.2831 * v.x);
	// s.y *= 0.5 * cos(iTime + 6.2831 * v.y);

	float m1 = mix( dot( hash2(p1), fr - vec2(0.0,0.0) ), dot( hash2(p2), fr - vec2(1.0,0.0) ), s.x);
	float m2 = mix( dot( hash2(p3), fr - vec2(0.0,1.0) ), dot( hash2(p4), fr - vec2(1.0,1.0) ), s.x);
   
	noise = mix(m1, m2, s.y);

	// Your implementation ends here
	return noise;
}

///////////// Part 1c /////////////////////
/*  Given a point v and an int num, compute the perlin noise octave for point v with octave num
	num will be greater than 0 */
float noiseOctave(vec2 v, int num)
{
	// use differet frequencies
	float sum = 0;
	// Your implementation starts here
	for (int i = 0; i < num; i ++){
		float w = pow(2.0, -1.0 * i);
		float s = pow(2.0, i);
		sum += w * perlin_noise(s * v);
	}
	
	// Your implementation ends here
	return sum;
}

///////////// Part 2a /////////////////////
/* create a function that takes in a 2D point and returns a height using perlin noise 
    There is no right answer. Think about what functions will create what shapes.
    If you want steep mountains with flat tops, use a function like sqrt(noiseOctave(v,num)). 
    If you want jagged mountains, use a function like e^(noiseOctave(v,num))
    You can also add functions on top of each other and change the frequency of the noise
    by multiplying v by some value other than 1*/
float height(vec2 v){
    float h = 0;
	// Your implementation starts here

	h = pow(0.3, noiseOctave(v, 3)) / 8 + 0.8; 

	// Your implementation ends here
	return h;
}

/*input variables*/
layout (location=0) in vec4 pos;		/*vertex position*/
layout (location=1) in vec4 color;		/*vertex color*/
layout (location=2) in vec4 normal;		/*vertex normal*/	
layout (location=3) in vec4 uv;			/*vertex uv*/		
layout (location=4) in vec4 tangent;	/*vertex tangent*/	

/*output variables*/
out vec3 vtx_pos;		////vertex position in the world space


void main()												
{
	vtx_pos = (vec4(pos.xy, height(pos.xy), 1)).xyz;
	gl_Position = pvm * vec4(vtx_pos.xyz, 1.f);

}	
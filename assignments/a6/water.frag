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
// uniform float iTime;					////time
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal
// uniform vec2 iResolution;

/*uniform variables*/
uniform float iTime;					////time
uniform vec2 iResolution;

/*input variables*/
in vec3 vtx_pos;

/*input variables*/
out vec4 frag_color;

///////////// Part 1a /////////////////////
/* Create a function that takes in an xy coordinate and returns a 'random' 2d vector. (There is no right answer)
   Feel free to find a hash function online. Use the commented function to check your result */
vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	// Your implementation starts here
	//Source : https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner/
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

///////////// Part 2b /////////////////////
/* compute the normal vector at v by find the points d to the left/right and d forward/backward 
    and using a cross product. Be sure to normalize the result */
vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	// Your implementation starts here
	vec3 v1 = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y))); // right
	vec3 v2 = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y))); // left
	vec3 v3 = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d))); // top
	vec3 v4 = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d))); // bottom

	vec3 horizontal = v1 - v2;  // right - left
	vec3 vertical = v3 - v4; // top - bottom

	normal_vector = normalize(cross(horizontal, vertical));
	
	// Your implementation ends here
	return normal_vector;
}

///////////// Part 2c /////////////////////
/* complete the get_color function by setting emissiveColor using some function of height/normal vector/noise */
/* put your Phong/Lambertian lighting model here to synthesize the lighting effect onto the terrain*/
vec3 get_color(vec2 v)
{
	vec3 vtx_normal = compute_normal(v, 0.01);
	vec3 PURPLE_BLUE = vec3(0.1, 0.0, 0.5);
	vec3 PURPLE = vec3(0.2, 0.0 , 0.5);
	vec3 PINK = vec3(0.8, 0.3 , 0.6);
	vec2 st = gl_FragCoord.xy/iResolution.xy;
	
	float blend = height(v);
	float changeColor = abs(sin(iTime));
	vec3 emissiveColor = mix(mix(PINK, PURPLE_BLUE, blend * 2), mix(PURPLE_BLUE, PURPLE, blend ), changeColor);
	vec3 lightingColor= vec3(1.,1.,1.);

	// Your implementation starts here
	/*This part is the same as your previous assignment. Here we provide a default parameter set for the hard-coded lighting environment. Feel free to change them.*/
	const vec3 LightPosition = vec3(3, 1, 4);
	const vec3 LightIntensity = vec3(20);
	const vec3 ka = 0.1*vec3(1., 1., 1.);
	const vec3 kd = 0.7*vec3(1., 1., 1.);
	const vec3 ks = vec3(1.);
	const float n = 400.0;

	vec3 normal = normalize(vtx_normal.xyz);
	vec3 lightDir = LightPosition - vtx_pos;
	float _lightDist = length(lightDir);
	vec3 _lightDir = normalize(lightDir);
	vec3 _localLight = LightIntensity / (_lightDist * _lightDist);
	vec3 ambColor = ka;
	lightingColor = kd * _localLight * max(0., dot(_lightDir, normal));

	// Your implementation ends here

    return emissiveColor*lightingColor;


}

void main()
{
	frag_color = vec4(get_color(vtx_pos.xy),1.f);
}


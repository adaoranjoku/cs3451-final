//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#include <iostream>

#include <random>
#include "Common.h"
#include "Driver.h"
#include "Particles.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLParticles.h"

/////////////////////////////////////////////////////////////////////
//// TODO: put your name in the string               
/////////////////////////////////////////////////////////////////////

const std::string author="name";

/////////////////////////////////////////////////////////////////////
//// These are helper functions we created to generate circles and triangles by testing whether a point is inside the shape or not.
//// They can be used in the paintGrid function as "if the pixel is inside, draw some color; else skip."
//// You may create your own functions to draw your own shapes

//// The paintGrid function is implemented as a GLSL fragment shader. 
//// The GLSL grammar is C-style, and if you are curious about its full picture (which we will start to learn the details in Week 3), 
//// you may find more information on https://www.khronos.org/files/opengl43-quick-reference-card.pdf (Page 6 - 7 would probably be helpful!)
//// You don't need advanced GLSL features for this assignment (reading the starter code should be enough).
//// You can also test it (copy the whole string) in Shadertoy: https://www.shadertoy.com/new    
/////////////////////////////////////////////////////////////////////

const std::string draw_pixels = To_String(
	const float M_PI = 3.1415926535;
const float PIXEL_SIZE = 10.;

// Reference: Ray Marching for Dummies: https://www.shadertoy.com/view/XlGBW3.

//////////////////////////////////////////////////////////////////////////
///					    Original license information                   ///
//////////////////////////////////////////////////////////////////////////
// "ShaderToy Tutorial - Ray Marching for Dummies!" 
// by Martijn Steinrucken aka BigWings/CountFrolic - 2018
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
//
// This shader is part of a tutorial on YouTube
// https://youtu.be/PGtv-dBi2wE



const int MAX_STEPS = 100;
const float MAX_DIST = 100.;
const float SURF_DIST = .01;

struct Ray
{
	/// Ray origin.
	vec3 o;

	/// Normalized ray direction.
	vec3 d;
};

// GLSL does not support inheritance, constructors (while HLSL do!).
// So I emulate by adding a prefix "Shape" to a concrete shape type.
struct ShapeSphere
{
	/// Sphere origin.
	vec3  origin;

	/// Sphere radius.
	float radius;
};

// Default specifier is "in".
float SDF_Sphere(ShapeSphere sphere, vec3 p)
{
	return length(p - sphere.origin) - sphere.radius;
}

struct ShapePlane
{
	/// Plane normal.
	vec3 normal;

	/// Distance to origin.
	float h;
};

// SDF Reference: https://iquilezles.org/articles/distfunctions/.
// Axis aligned plane placed at Y;
float SDF_Plane(ShapePlane plane, vec3 p)
{
	// Note: this would follow directly from the formula (distance of a point to plane).
	return dot(p, plane.normal) + plane.h;
}

float SDF_Box(vec3 p, vec3 s) {
	return length(max(abs(p) - s, 0.));
}

float sdOctahedron(vec3 p, float s)
{
	p = abs(p);
	float m = p.x + p.y + p.z - s;
	vec3 r = 3.0 * p - m;

	// my original version
	vec3 q;
	if (r.x < 0.0) q = p.xyz;
	else if (r.y < 0.0) q = p.yzx;
	else if (r.z < 0.0) q = p.zxy;
	else return m * 0.57735027;
	float k = clamp(0.5 * (q.z - q.y + s), 0.0, s);
	return length(vec3(q.x, q.y - s + k, q.z - k));
}

float sdCapsule(vec3 p, vec3 a, vec3 b, float r) {
	vec3 ab = b - a;
	vec3 ap = p - a;

	float t = dot(ab, ap) / dot(ab, ab);
	t = clamp(t, 0., 1.);

	vec3 c = a + t * ab;

	return length(p - c) - r;
}

float sdBox(vec3 p, vec3 s) {
	return length(max(abs(p) - s, 0.));
}

float map(in vec3 pos)
{
	// Mapping reference: https://www.shadertoy.com/view/wsSGDG.
	float rad = 0.1 * (0.5 + 0.5 * sin(iTime * 2.0));
	return sdOctahedron(pos, 0.5 - rad) - rad;
}

/// @brief SDF implementation. Return the closest distance \t at \p to the surface.
/// @param p 
/// @return Distance t
float SDF_EvaluateDistance(vec3 p) {
	// RGB => sphere position
	// A   => radius
	ShapeSphere sphere = ShapeSphere(vec3(0, 1, 6), 1.0);
	ShapePlane  plane = ShapePlane(vec3(0, 1, 0), 0.0);

	float sphereDist = SDF_Sphere(sphere, p);
	float planeDist = SDF_Plane(plane, p);
	float octahedronDist = map(p - vec3(1.5, 0.5, 3.5));
	float capsuleDist = sdCapsule(p, vec3(3, .5, 6), vec3(3, 2.5, 6), .5);
	float boxDist = sdBox(p - vec3(-3.0, 0.5, 6.0), vec3(0.5, 0.5, 0.7));

	float t = min(sphereDist, planeDist);
	t = min(t, octahedronDist);
	t = min(t, capsuleDist);
	t = min(t, boxDist);
	return t;
}

/// @brief Use ray marching to find the closest hit for \ray.
/// @param ray 
/// @return Closest hit parametric distance for ray (Recall ray's parametric definition: ray(t) = o + t * d)
float ClosestHit_RayMarching(Ray ray)
{
	// We always start from 0.
	// You could also put this into \Ray type so your ray could have a minimal starting distance.
	float t = 0.;

	// Ray march finite steps.
	for (int i = 0; i < MAX_STEPS; i++)
	{
		// Compute our new marched point in the ray.
		vec3 p = ray.o + ray.d * t;

		// Use SDF to find out the maximum marching distance.
		float dS = SDF_EvaluateDistance(p);

		// Ray marching!
		t += dS;

		// Termination condition:
		// 1. If we go pretty far and cannot still find a closest hit point, it is possible
		//    that we could never have a hit (e.g. our ray starts behind the primitive; nothing to intersect)
		// 2. We do find a closest hit.
		if (t > MAX_DIST || dS < SURF_DIST)
			break;
	}

	// Return our closest hit distance.
	return t;
}

/// @brief Compute normal at the intersection point.
/// @param p intersection point
/// @return normal (always normalized)
vec3 Intersection_GetNormal(vec3 p) {
	// Reference: https://iquilezles.org/articles/normalsSDF/.
	// Use backward difference to compute the gradient (which has the same value with normal) at p.
	float f_p = SDF_EvaluateDistance(p);

	// epsilon used for finite difference.
	vec2 e = vec2(.01, 0);

	// Forward diff: df(p)/dx ~= (f(p + {epsilon, 0, 0}) - f(p)) / epsilon
	// Backward diff: df(p)/dx ~= (f(p) - f(p - {epsilon, 0, 0})) / epsilon
	// We could safely ignore divided by epsilon since we will normalize the gradient anyway.
	vec3 dfp_dx = f_p - vec3(
		SDF_EvaluateDistance(p - e.xyy),
		SDF_EvaluateDistance(p - e.yxy),
		SDF_EvaluateDistance(p - e.yyx));

	return normalize(dfp_dx);
}

/// @brief Compute Direct Lighting for at a point p.
/// @param p Position in world.
/// @return Color.
vec3 Integrator_EstimateDirectLighting(vec3 p) {
	// point Light position.
	vec3 lightPos = vec3(0, 5, 6);
	lightPos.xz += vec2(sin(iTime), cos(iTime)) * 2.;

	// point light intensity.
	vec3 lightIntensity = vec3(1, 1, 1);

	// compute direct lighting.
	vec3 wi = normalize(lightPos - p);
	vec3 n = Intersection_GetNormal(p);

	// lambert's law.
	float absCosine = clamp(dot(n, wi), 0., 1.);

	// Send shadow ray.
	Ray shadowRay = Ray(p + n * SURF_DIST * 2. /* specify an offset to avoid self intersection*/,
		wi               /* ray direction */);

	// Find a closest hit point.
	float t = ClosestHit_RayMarching(shadowRay);

	// Since our ray's direction is normalized, t is the distance.
	// If our closest hit distance is less than the light's distance, we are blocked.
	if (t < length(lightPos - p))
	{
		return vec3(0, 0, 0);
	}

	return absCosine * lightIntensity;
}



/// You can define your own camera functions (orthographical view for example)!

/// @brief Camera related function for ray generation.
/// @param uv "Normalized" uv coordinates. 
/// @remark Our UV coordinate convention: 
///         V should always be in [-0.5, 0.5],
///         U should be in [-0.5, 0.5] 
///            if aspect ratio == 1, 
///            else scale by aspect ratio.
/// @return Generated ray. Direction should always be normalized.
Ray Camera_Perspective_GenerateRay(in vec2 uv)
{
	Ray ray;
	// Y Up.
	ray.o = vec3(0, 1, 0);
	ray.d = normalize(vec3(uv.x, uv.y, 1));

	return ray;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	/// Step 1: Generate a ray.
	// Normalize UV, move it into the center and account for the aspect ratio.
	vec2 uv = (fragCoord - .5 * iResolution.xy) / iResolution.y;
	Ray cameraRay = Camera_Perspective_GenerateRay(uv);

	/// Step 2: Ray-scene intersection.
	// Ray scene intersect by ray marching.
	// We will find a closest hit point for the ray.
	float rayT = ClosestHit_RayMarching(cameraRay);

	// This is our closest hit point.
	vec3 p = cameraRay.o + cameraRay.d * rayT;

	/// Step 3: Shading.
	// 
	vec3 col = Integrator_EstimateDirectLighting(p);

	/// Step 4: Postprocessing.
	// Apply gamma correction to ensure linear workflow.
	col = pow(col, vec3(.4545));

	// Write to output framebuffer.
	fragColor = vec4(col, 1.0);
}
);

class AX_RayMarch_SDF_Driver : public Driver, public OpenGLViewer
{
	OpenGLScreenCover* screen_cover = nullptr;
	clock_t startTime = clock();

public:
	virtual void Initialize()
	{
		OpenGLViewer::Initialize();
	}

	//// Initialize the screen covering mesh and shaders
	virtual void Initialize_Data()
	{
		OpenGLShaderLibrary::Instance()->Create_Screen_Shader(draw_pixels, "shaderToy");
		screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
		Set_Polygon_Mode(screen_cover, PolygonMode::Fill);
		Uniform_Update();

		screen_cover->Set_Data_Refreshed();
		screen_cover->Initialize();
		screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shaderToy"));
	}

	//// Update the uniformed variables used in shader
	void Uniform_Update()
	{
		screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
		screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		Uniform_Update();
		OpenGLViewer::Toggle_Next_Frame();
	}

	////Keyboard interaction
	virtual void Initialize_Common_Callback_Keys()
	{
		OpenGLViewer::Initialize_Common_Callback_Keys();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	if(author==""){std::cerr<<"***** The author name is not specified. Please put your name in the author string first. *****"<<std::endl;return 0;}
	else std::cout<<"Ray Marching SDF demo by "<<author<<" started"<<std::endl;

	AX_RayMarch_SDF_Driver driver;
	driver.Initialize();
	driver.Run();	
}


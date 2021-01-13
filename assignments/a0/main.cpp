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

#define To_String(S) #S

template<int d> class A0_Driver : public Driver, public OpenGLViewer
{
	using VectorD = Vector<real, d>; using VectorDi = Vector<int, d>; using Base = Driver;
	OpenGLScreenCover* screen_cover = nullptr;
	clock_t startTime = clock();

	const std::string draw_pixels= To_String(
		void mainImage(out vec4 fragColor, in vec2 fragCoord)
		{
			////////////////////////////////////////////////////////////////
			// Replace the code below with your own code                  //
		    // Recently only iResolution and iTime are supported as Uniforms //
			////////////////////////////////////////////////////////////////

			// Normalized pixel coordinates (from 0 to 1)
			vec2 uv = fragCoord / iResolution.xy;
			// Time varying pixel color
			vec3 col = 0.5 + 0.5*cos(iTime + uv.xyx + vec3(0, 2, 4));
			// Output to screen
			fragColor = vec4(col, 1.0);
			//////////////////////////////////////////////
		}
	);

public:
	virtual void Initialize()
	{
		OpenGLViewer::Initialize();
	}

	/// Called when initialized
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

	//// updated the uniformed variables used in shader
	void Uniform_Update()
	{
		screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
		screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
	}

	////update variables for each time step
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
	int driver=1;

	switch(driver){
	case 1:{
		A0_Driver<3> driver;
		driver.Initialize();
		driver.Run();	
	}break;
	}
	
}


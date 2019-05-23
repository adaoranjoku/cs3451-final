//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, technical project starter code
//#####################################################################
#include <iostream>

#include "Common.h"
#include "Driver.h"
#include "Particles.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLParticles.h"
#include "TinyObjLoader.h"

#include "Module.h"
#include "Rules.h"
#include "Rule.h"
#include "System.h"
#include <List>

class TechProjDriver : public Driver, public OpenGLViewer
{
	using Base = Driver;
public:
	Array<TriangleMesh<3>* > triangle_meshes;

	virtual void Initialize()
	{
		OpenGLViewer::Initialize();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}

	////Initialize data in the scene
	virtual void Initialize_Data()
	{
		Init_Shaders();
		//Init_Background();

		Init_Tree_Mesh();

		Init_Lighting();
	}

	//////////////////////////////////////////////////////////////////////////
	////Specifying your own lighting effects in this function
	////In this example we initialized two directional lights, the same as Assignment 2
	void Init_Lighting() {
		auto dir_light = OpenGLUbos::Add_Directional_Light(glm::vec3(-1.f, -1.f, -1.f));
		dir_light->dif = glm::vec4(.9,.8,.7, 1.0);

		dir_light = OpenGLUbos::Add_Directional_Light(glm::vec3(1.f, -1.f, -1.f));
		dir_light->dif = glm::vec4(.7,.7,.7, 1.0);

		OpenGLUbos::Set_Ambient(glm::vec4(.01f, .01f, .02f, 1.f));
		OpenGLUbos::Update_Lights_Ubo();	
	}

	//////////////////////////////////////////////////////////////////////////
	////Write your own vertex shader and fragment shader and add it to the shader library
	void Init_Shaders()
	{
		Add_Shader("shaders/background.vert", "shaders/background.frag", "background");
		Add_Shader("shaders/tree.vert","shaders/tree.frag","l_sys");
	}

	//////////////////////////////////////////////////////////////////////////
	////Initialize the background shader
	void Init_Background()
	{
		OpenGLBackground* opengl_background=Add_Interactive_Object<OpenGLBackground>();
		opengl_background->shader_name="background";
		opengl_background->Initialize();
	}

	void Init_Tree_Mesh() 
	{
		////Initialize the mesh file, shader, and texture of the mesh
		std::string mesh_file_name = "models/v2_high_poly.obj";
		std::string shader_name = "l_sys";

		////Read mesh from obj file
		OpenGLTriangleMesh* opengl_tri_mesh = Add_Interactive_Object<OpenGLTriangleMesh>();
		Read_Mesh(mesh_file_name, opengl_tri_mesh->mesh);

		////Initialize the model matrix
		opengl_tri_mesh->model_matrix = glm::mat4(1.0f);
		opengl_tri_mesh->taper_ratio = 0.8f;

		////Other mesh initialization setups
		Set_Mesh_Default_Options(opengl_tri_mesh);

		////Bind an initialized shader to the mesh
		opengl_tri_mesh->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader(shader_name));

		////Add the triangle mesh to the array to access the mesh later
		TriangleMesh<3>* triangle_mesh=&opengl_tri_mesh->mesh;
		triangle_meshes.push_back(triangle_mesh);
	}

	void Sync_Simulation_And_Visualization_Data()
	{
		////if you want to animate your meshes or objects, implement this function
		
	}

	////update simulation and visualization for each time step
	virtual void Toggle_Next_Frame()
	{
		Sync_Simulation_And_Visualization_Data();
		OpenGLViewer::Toggle_Next_Frame();
	}

	////Keyboard interaction
	virtual void Initialize_Common_Callback_Keys()
	{
		OpenGLViewer::Initialize_Common_Callback_Keys();
		Bind_Callback_Key('a',&Keyboard_Event_A_Func,"press A");
	}

	////Implement your own keyboard function, 
	virtual void Keyboard_Event_A()
	{
		std::cout<<"A pressed"<<std::endl;
	}
	Define_Function_Object(TechProjDriver,Keyboard_Event_A);

protected:
	void Set_Mesh_Default_Options(OpenGLTriangleMesh* opengl_tri_mesh)
	{
		Set_Polygon_Mode(opengl_tri_mesh, PolygonMode::Fill);
		Set_Shading_Mode(opengl_tri_mesh, ShadingMode::Custom);
		opengl_tri_mesh->Set_Data_Refreshed();
		opengl_tri_mesh->Initialize();
	}

	//////////////////////////////////////////////////////////////////////////
	////Add a shader to the shader library
	void Add_Shader(const std::string vtx_shader_file_name,const std::string frag_shader_file_name,const std::string shader_name)
	{
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vtx_shader_file_name,frag_shader_file_name,shader_name);
	}

	//////////////////////////////////////////////////////////////////////////
	////Add a texture to the texture library
	void Add_Texture(const std::string texture_file_name,const std::string texture_name)
	{
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File(texture_file_name, texture_name);
	}

	//////////////////////////////////////////////////////////////////////////
	////Read an obj mesh
	void Read_Mesh(const std::string mesh_file_name,TriangleMesh<3>& read_mesh)
	{
		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File(mesh_file_name, meshes);
		read_mesh=*meshes[0];
	}

	//////////////////////////////////////////////////////////////////////////
	////Calculate the bounding box of a set of vertices
	void Bounding_Box(const Array<Vector3>& vertices,Vector3& min_corner,Vector3& max_corner)
	{
		min_corner=Vector3::Ones()*(1e7);max_corner=Vector3::Ones()*(-1e7);
		for(auto& v:vertices){
			min_corner=min_corner.cwiseMin(v);
			max_corner=max_corner.cwiseMax(v);}
	}

	//////////////////////////////////////////////////////////////////////////
	////Rescale the points to a box with longest_length
	void Rescale(Array<Vector3>& vertices,const real longest_length)
	{
		Vector3 min_corner,max_corner;
		Bounding_Box(vertices,min_corner,max_corner);
		Vector3 length=max_corner-min_corner;
		int axis=0;if(length[1]>length[axis])axis=1;if(length[2]>length[axis])axis=2;
		real rescale=(length[axis]>(real)0)?longest_length/length[axis]:(real)1;for(auto& v:vertices)v*=rescale;
	}

	//////////////////////////////////////////////////////////////////////////
	////Calculate the center
	Vector3 Center(const Array<Vector3>& vertices)
	{
		Vector3 sum=Vector3::Zero();for(auto& v:vertices)sum+=v;
		return sum/=(real)vertices.size();
	}

	//////////////////////////////////////////////////////////////////////////
	////Translate the center to the target position
	void Translate_Center_To(Array<Vector3>& vertices,const Vector3 target)
	{
		Vector3 center=Center(vertices);
		for(auto& v:vertices)v+=(target-center);
	}

	void Subdivide(TriangleMesh<3>* mesh)
	{
		Hashtable<Vector2i,int> edge_vtx_hashtable;
		for(int k=0;k<mesh->elements.size();k++){
			for(int j=0;j<3;j++){
				Vector2i e(mesh->elements[k][j%3],mesh->elements[k][(j+1)%3]);
				if(e[0]<e[1])continue;
				Vector3 pos=(real).5*(mesh->Vertices()[e[0]]+mesh->Vertices()[e[1]]);
				mesh->Vertices().push_back(pos);
				int i=(int)mesh->Vertices().size()-1;
				edge_vtx_hashtable.insert(std::make_pair(e,i));
			}
		}

		auto n=mesh->elements.size();
		for(auto i=0;i<n;i++){const Vector3i v=mesh->elements[i];int v3,v4,v5;
			{auto search=edge_vtx_hashtable.find(Sorted(Vector2i(v[0],v[1])));if(search==edge_vtx_hashtable.end())continue;v3=search->second;}
			{auto search=edge_vtx_hashtable.find(Sorted(Vector2i(v[1],v[2])));if(search==edge_vtx_hashtable.end())continue;v4=search->second;}
			{auto search=edge_vtx_hashtable.find(Sorted(Vector2i(v[2],v[0])));if(search==edge_vtx_hashtable.end())continue;v5=search->second;}
			mesh->elements.push_back(Vector3i(v[0],v3,v5));
			mesh->elements.push_back(Vector3i(v3,v[1],v4));
			mesh->elements.push_back(Vector3i(v5,v4,v[2]));
			mesh->elements[i]=Vector3i(v3,v4,v5);}
	}
};

int main(int argc,char* argv[])
{
	std::map<char, int> alphabet;
	alphabet['A'] = 2;
	alphabet['B'] = 1;
	alphabet['C'] = 0;
	std::string axiom = "B(2)A(4,4)";
	std::map<char, Rules> productions;
	productions['A'] = Rules();
	productions['B'] = Rules();
	productions['C'] = NullRules();

	class ARule1 : public Rule {
		virtual bool satisfied(Module m) const override { return m.param(1) <= 3; }
		virtual std::list<Module> parse(Module m) const override {
			std::list<Module> modules = std::list<Module>();
			modules.push_back(Module('A', { m.param(0) * 2, m.param(0) + m.param(1) }));
			return modules;
		}
	};
	class ARule2 : public Rule {
		virtual bool satisfied(Module m) const override { return m.param(1) > 3; }
		virtual std::list<Module> parse(Module m) const override {
			std::list<Module> modules = std::list<Module>();
			modules.push_back(Module('B', { m.param(0) }));
			modules.push_back(Module('A', { m.param(0) / m.param(1), 0.f }));
			return modules;
		}
	};
	productions['A'].addRule(new ARule1());
	productions['A'].addRule(new ARule2());

	class BRule1 : public Rule {
		virtual bool satisfied(Module m) const override { return m.param(0) < 1; }
		virtual std::list<Module> parse(Module m) const override {
			std::list<Module> modules;
			modules.push_back(Module('C'));
			return modules;
		}
	};
	class BRule2 : public Rule {
		virtual bool satisfied(Module m) const override { return m.param(0) >= 1; }
		virtual std::list<Module> parse(Module m) const override {
			std::list<Module> modules;
			modules.push_back(Module('B', {m.param(0) - 1}));
			return modules;
		}
	};
	productions['B'].addRule(new BRule1());
	productions['B'].addRule(new BRule2());

	System algae = System(alphabet, axiom, productions);
	std::cout << std::endl << algae.toString() << std::endl << std::flush;
	for (int i = 0; i < 4; i++) {
		algae.iterate();
		std::cout << std::endl << algae.toString() << std::endl << std::flush;
	}
	
	TechProjDriver driver;
	driver.Initialize();
	driver.Run();
}


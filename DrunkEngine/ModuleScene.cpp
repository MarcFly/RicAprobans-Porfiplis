#include "ModuleScene.h"
#include "Application.h"
#include "ConsoleWindow.h"
#include "ModuleCamera3D.h"
#include "SDL/include/SDL_opengl.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "GeoPropertiesWindow.h"


#include "ModuleRenderer3D.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DevIL/libx86/Release/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/Release/ILU.lib")

void CallLog(const char* str, char* usrData);

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

bool ModuleScene::Init()
{
	bool ret = true;

	// DevIL initialization
	ilInit();
	iluInit();

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = CallLog;
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleScene::Start()
{
	bool ret = true;

	Load(nullptr);
	LoadFromFile("./BakerHouse.fbx");

	return ret;
}

bool ModuleScene::CleanUp()
{
	bool ret = false;

	PLOG("Destroying all objects");

	ret = DestroyScene();

	// detach log streamW
	aiDetachAllLogStreams();

	return ret;
}

bool ModuleScene::LoadFromFile(const char* file_path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_Fast);// for better looks i guess: aiProcessPreset_TargetRealtime_MaxQuality);
	std::string aux = file_path;

	if (scene == nullptr)
	{
		// Trying to load it from the scene folder
		std::string new_file_path = file_path;
		new_file_path = new_file_path.substr(new_file_path.find_last_of("\\/") + 1);

		new_file_path = scene_folder + new_file_path;

		scene = aiImportFile(new_file_path.c_str(), aiProcessPreset_TargetRealtime_Fast);
		aux = new_file_path;
	}
	else
		App->ui->console_win->AddLog("Failed to Load from file %s", file_path);

	if (scene != nullptr)
	{
		DestroyScene();
		GameObject* root = new GameObject(scene, scene->mRootNode, aux.substr(aux.find_last_of("\\/") + 1).c_str());
		
		aiReleaseImport(scene);
	}
	else
	{
		App->ui->console_win->AddLog("Error loading scene's meshes %s", file_path);
		ret = false;
	}

	return ret;
}

void ModuleScene::DrawMesh(const ComponentMesh* mesh, bool use_texture) 
{
	// Draw elements
	{

		glColor4f(1, 1, 1, 1);

		// Bind buffers
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);

		glColor4f(1, 1, 1, 1);

		if (use_texture)
		{
			if(mesh->parent->textures.size() > 0 && mesh)
			int test = mesh->parent->textures.size() - 1;
			if(mesh->parent->textures.size() > 0 && mesh->tex_index <= (mesh->parent->textures.size() - 1))
			{ 
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
				glTexCoordPointer(3, GL_FLOAT, 0, NULL);

				glBindTexture(GL_TEXTURE_2D, mesh->parent->textures[mesh->tex_index].id_tex);
			}
			else if (mesh->parent->mat_colors.size() > 0)
			{
				Color c = mesh->parent->mat_colors[mesh->tex_index];
				glColor4f(c.r, c.g, c.b, c.a);
			}
		}
		
		// Draw
		glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, NULL);

		// Unbind Buffers
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glColor4f(0, 1, 0, 1);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	//DrawBB(mesh);
}

bool ModuleScene::Load(JSON_Value * root_value)
{
	bool ret = false;

	root_value = json_parse_file("config_data.json");

	scene_folder = json_object_dotget_string(json_object(root_value), "manage_mesh.scenes_path");
	tex_folder = json_object_dotget_string(json_object(root_value), "manage_mesh.materials_path");

	
	ret = true;
	return ret;
}

bool ModuleScene::Save(JSON_Value * root_value)
{
	bool ret = false;

	root_value = json_parse_file("config_data.json");
	JSON_Object* root_obj = json_value_get_object(root_value);

	

	json_serialize_to_file(root_value, "config_data.json");

	App->ui->console_win->AddLog("Texture config saved");

	ret = true;
	return ret;
}

//-SET OBJ DATA------------------------------------------------------------------------------------------

// -----------------------
int ModuleScene::GetDevILVer()
{
	return IL_VERSION;
}

void ModuleScene::DestroyObject(const int& index)
{
	for (int i = 0; i < Objects[index].meshes.size(); i++) {
		glDeleteBuffers(1, &Objects[index].meshes[i].id_index);
		//glDeleteBuffers(GL_ARRAY_BUFFER, &Objects[index].meshes[i].id_normal);
		glDeleteBuffers(1, &Objects[index].meshes[i].id_uvs);
		glDeleteBuffers(1, &Objects[index].meshes[i].id_vertex);

		delete Objects[index].meshes[i].index;
		delete Objects[index].meshes[i].normal;
		delete Objects[index].meshes[i].tex_coords;
		delete Objects[index].meshes[i].vertex;

		if (Objects[index].mBoundingBody != nullptr) {
			Objects[index].mBoundingBody->DelMathBody();
			delete Objects[index].mBoundingBody;
		}
	}
	Objects[index].meshes.clear();

	for (int i = 0; i < Objects[index].materials.size(); i++)
	{
		glDeleteTextures(1, &Objects[index].materials[i].id_tex);
	}
	Objects[index].materials.clear();

	Objects[index].mat_colors.clear();

	Objects.erase(Objects.begin(), Objects.begin());
}

void ModuleScene::DestroyTexture(GameObject* curr_obj, const int& tex_ind)
{
	glDeleteTextures(1, &curr_obj->textures[tex_ind].id_tex);

	for (int i = tex_ind + 1; i < curr_obj->textures.size(); i++)
	{
		curr_obj->textures[i - 1] = curr_obj->textures[i];
	}
	curr_obj->textures.pop_back();


	if (curr_obj->textures.size() < 1)
		for (int i = 0; i < curr_obj->meshes.size(); i++)
			curr_obj->meshes[i].tex_index = 0;
	else
		for (int i = 0; i < curr_obj->meshes.size(); i++)
			if (curr_obj->meshes[i].tex_index >= curr_obj->textures.size())
				curr_obj->meshes[i].tex_index = curr_obj->textures.size() - 1;
			
}





void ModuleScene::SetCurrTexTo(GameObject& curr_obj, const int tex_ind)
{
	for (int i = 0; i < curr_obj.meshes.size(); i++)
		curr_obj.meshes[i].tex_index = tex_ind;
}

float ModuleScene::SetObjBoundBox(GameObject &object, const aiScene* scene)
{
	float ret = 0;

	object.BoundingBox.maxPoint = vec(INT_MIN, INT_MIN, INT_MIN);
	object.BoundingBox.minPoint = vec(INT_MAX, INT_MAX, INT_MAX);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		// Setting the BB min and max points

		if (object.BoundingBox.maxPoint.x < object.meshes[i].BoundingBox.maxPoint.x)
			object.BoundingBox.maxPoint.x = object.meshes[i].BoundingBox.maxPoint.x;

		if (object.BoundingBox.minPoint.x > object.meshes[i].BoundingBox.minPoint.x)
			object.BoundingBox.minPoint.x = object.meshes[i].BoundingBox.minPoint.x;

		if (object.BoundingBox.maxPoint.y < object.meshes[i].BoundingBox.maxPoint.y)
			object.BoundingBox.maxPoint.y = object.meshes[i].BoundingBox.maxPoint.y;

		if (object.BoundingBox.minPoint.y > object.meshes[i].BoundingBox.minPoint.y)
			object.BoundingBox.minPoint.y = object.meshes[i].BoundingBox.minPoint.y;

		if (object.BoundingBox.maxPoint.z < object.meshes[i].BoundingBox.maxPoint.z)
			object.BoundingBox.maxPoint.z = object.meshes[i].BoundingBox.maxPoint.z;

		if (object.BoundingBox.minPoint.z > object.meshes[i].BoundingBox.minPoint.z)
			object.BoundingBox.minPoint.z = object.meshes[i].BoundingBox.minPoint.z;
	}

	{
		if (abs(object.BoundingBox.maxPoint.x) > ret) {ret = abs(object.BoundingBox.maxPoint.x);}
		if (abs(object.BoundingBox.maxPoint.y) > ret) {ret = abs(object.BoundingBox.maxPoint.y);}
		if (abs(object.BoundingBox.maxPoint.z) > ret) {ret = abs(object.BoundingBox.maxPoint.z);}
		if (abs(object.BoundingBox.minPoint.x) > ret) {ret = abs(object.BoundingBox.minPoint.x);}
		if (abs(object.BoundingBox.minPoint.y) > ret) {ret = abs(object.BoundingBox.minPoint.y);}
		if (abs(object.BoundingBox.minPoint.z) > ret) {ret = abs(object.BoundingBox.minPoint.z);}
	}

	return ret;

}

// CREATE PRIMITIVE OBJECTS -------------------------------------------------------------------------------
/*
bool ModuleScene::CreatePrimitiveObject(const vec& center, PCube& cube)
{
	bool ret = true;

	GameObject add_obj;

	PCube* new_cube = new PCube(cube);

	new_cube->MathBody = new AABB(Sphere(center, cube.size.x/2.0f));

	ComponentMesh mcube;
	mcube.num_vertex = new_cube->MathBody->NumVertices();
	mcube.vertex = new float[mcube.num_vertex*3];
	memcpy(mcube.vertex, &new_cube->MathBody->CornerPoint(0), 3 * sizeof(float)*mcube.num_vertex);

	std::vector<int> vert_order =
	{	0,3,2,	0,1,3,
		1,5,7,	1,7,3,
		5,4,7,	4,6,7,
		4,0,2,	4,2,6,
		2,3,7,	2,7,6,
		0,4,5,	0,5,1
	};

	mcube.num_index = new_cube->MathBody->NumFaces() * 3;
	mcube.index = new GLuint[mcube.num_index];
	memcpy(mcube.index, &vert_order[0], 3 * sizeof(GLuint)*mcube.num_vertex);

	mcube.num_normal = mcube.num_index * 2;
	mcube.normal = new float[mcube.num_normal];

	for (int i = 0; i < mcube.num_index / 3; i++)
		SetNormals(mcube, i);

	std::vector<float> tex_coords =
	{
		0,0,0,	1,1,0,	0,1,0,
		0,0,0,	1,0,0,	1,1,0,
		0,0,0,	1,0,0,	1,1,0,
		0,0,0,	1,1,0,	0,1,0,
		0,0,0,	1,0,0,	0,1,0,
		1,0,0,	1,1,0,	0,1,0,
		0,0,0,	1,0,0,	1,1,0,
		0,0,0,	1,1,0,	0,1,0,
		0,0,0,	1,0,0,	1,1,0,
		0,0,0,	1,1,0,	0,1,0,
		0,0,0,	0,1,0,	1,1,0,
		0,0,0,	0,1,0,	1,1,0

	};

	mcube.num_uvs = tex_coords.size();
	mcube.tex_coords = new float[mcube.num_uvs];
	memcpy(mcube.tex_coords, &tex_coords[0], sizeof(float)*mcube.num_uvs);

	GenBuffers(mcube);

	add_obj.meshes.push_back(mcube);

	if (Objects.size() > 0)
	{
		DestroyObject(0);
		Objects.insert(Objects.begin(), add_obj);
		Objects.pop_back();
	}
	else
		Objects.push_back(add_obj);

	SetParents();

	return ret;
}
*/


void CallLog(const char* str, char* usrData)
{
	App->ui->console_win->AddLog(str);
}
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
#include "SceneViewerWindow.h"
#include "ModuleWindow.h"
#include "ComponentCamera.h"
#include "ModuleRenderer3D.h"
#include "FileHelpers.h"
#include "PrefabImport.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DevIL/libx86/Release/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/Release/ILU.lib")

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled, Type_Scene)
{

}

bool ModuleScene::Init()
{
	bool ret = true;

	return ret;
}

bool ModuleScene::Start()
{
	bool ret = true;


	// Setup for Delivery of Assignment 3
	//App->importer->LoadScene("./Assets/SceneAnim.drnk");

	App->renderer3D->OnResize();


	App->eventSys->Subscribe(EventType::Window_Resize, this);
	App->eventSys->Subscribe(EventType::Camera_Modified, this);



	return ret;
}

bool ModuleScene::Update(float dt)
{
	if (A3Animation != nullptr && A3Animation->anims.size() == 3)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			A3Animation->curr_animation = 2;
			A3Animation->prev_animation = 0;
			A3Animation->phase = BlendPhase::Start;
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			A3Animation->curr_animation = 1;
			A3Animation->prev_animation = 0;
			A3Animation->phase = BlendPhase::Start;
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP)
		{
			A3Animation->prev_animation = 1;
			A3Animation->curr_animation = 0;
			A3Animation->phase = BlendPhase::Start;
		}
	}

	return true;
}

bool ModuleScene::CleanUp()
{
	bool ret = false;

	PLOG("Destroying all objects");

	ret = App->gameObj->CleanUp();

	// detach log streamW
	aiDetachAllLogStreams();

	return ret;
}

bool ModuleScene::LoadFBX(const char* file_path)
{
	bool ret = true;
		
	App->importer->ExportScene(file_path);

	std::string drnkfile = ".\\Library\\" + GetFileName(file_path) + ".prefabdrnk";
	App->importer->LoadScene(drnkfile.c_str());
		
	App->gameObj->CreateMainCam();

	DebugTimer.LogTime(".prefabdrnk load");

	return ret;
}

bool ModuleScene::LoadSceneFile(const char* file_path)
{
	JSON_Value* scene = json_parse_file(file_path);
	JSON_Object* obj_g = json_value_get_object(scene);
	JSON_Array* gos = json_object_get_array(obj_g, "scene");

	App->gameObj->NewScene(App->importer->prefab_i->ImportGameObject(file_path, json_array_get_value(gos, 0)));

	for (int i = 1; i < json_array_get_count(gos); i++)
	{
		JSON_Value* val = json_array_get_value(gos, i);

		GameObject* add = App->importer->prefab_i->ImportGameObject(file_path, val);
		add->parent = App->gameObj->getRootObj();
		App->gameObj->getRootObj()->children.push_back(add);
	}

	App->gameObj->getRootObj()->OrderChildren();
	App->gameObj->getRootObj()->RecursiveSetNewUUID();

	Event evTrans(EventType::Transform_Updated, Event::UnionUsed::UseGameObject);
	evTrans.game_object.ptr = App->gameObj->getRootObj();
	App->eventSys->BroadcastEvent(evTrans);

	Event evCam(EventType::Update_Cam_Focus, Event::UnionUsed::UseGameObject);
	evCam.game_object.ptr = App->gameObj->getRootObj();
	App->eventSys->BroadcastEvent(evCam);

	return true;
}

bool ModuleScene::Load(const JSON_Value * root_value)
{
	bool ret = false;

	root_value = json_parse_file("config_data.json");

	JSON_Object* obj = json_value_get_object(root_value);
		
	scene_folder = json_object_dotget_string(obj, "scene.scenes_path");

	std::string default_load = json_object_dotget_string(obj, "scene.default_load");

	default_load = scene_folder + default_load;
			
	ret = true;
	return ret;
}

bool ModuleScene::Save(JSON_Value * root_value)
{
	bool ret = false;	

	JSON_Object* root_obj = json_value_get_object(root_value);
	// Write Module Scene config data to root_obj
	std::string Save_scene = "";
	if(App->gameObj->getRootObj() != nullptr)
		Save_scene = App->gameObj->getRootObj()->name + ".drnk";
	json_object_dotset_string(root_obj, "scene.default_load", Save_scene.c_str());
	json_object_dotset_string(root_obj, "scene.scenes_path", scene_folder.c_str());
	json_serialize_to_file(root_value, "config_data.json");

	ret = true;
	return ret;
}

std::string ModuleScene::SaveScene(const char* filename)
{
	GameObject* root = App->gameObj->getRootObj();

	if(filename != "")
		root->name = GetFileName(filename);

	CreateDirectory(".\\Assets\\", NULL);
	std::string Save_scene = ".\\Assets\\";
	if (App->gameObj->getRootObj() != nullptr)
		Save_scene += App->gameObj->getRootObj()->name + ".drnk";
	else
		Save_scene += "NullScene.drnk";

	if (App->gameObj->getRootObj() != nullptr)
	{
		JSON_Value* scene = json_parse_file(Save_scene.c_str());
		if (scene == nullptr)
		{
			scene = json_value_init_object();
			json_serialize_to_file(scene, Save_scene.c_str());
			scene = json_parse_file(Save_scene.c_str());
		}
		JSON_Value* set_array = json_value_init_array();
		JSON_Array* go = json_value_get_array(set_array);

		App->gameObj->getRootObj()->Save(go);

		JSON_Object* set = json_value_get_object(scene);
		json_object_set_value(set, "scene", set_array);

		json_serialize_to_file(scene, Save_scene.c_str());

		App->ui->console_win->AddLog("%s Scene saved", Save_scene.c_str());
	}

	return Save_scene;
}

//-SET OBJ DATA------------------------------------------------------------------------------------------

// -----------------------
int ModuleScene::GetDevILVer()
{
	return IL_VERSION;
}


void ModuleScene::RecieveEvent(const Event & event)
{
	switch (event.type)
	{
	case EventType::Camera_Modified:
	{
		break;
	}
	case EventType::Window_Resize:
	{
		break;
	}
	default:
		break;
	}
}

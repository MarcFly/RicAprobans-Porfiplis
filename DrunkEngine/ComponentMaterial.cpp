#include "ComponentMaterial.h"
#include "Application.h"
#include "ConsoleWindow.h"
#include "GeoPropertiesWindow.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ModuleImport.h"
#include "MaterialImport.h"

ComponentMaterial::ComponentMaterial(GameObject* par)
{
	SetBaseVals();

	parent = par;
}

void ComponentMaterial::DestroyTexture(const int& tex_ind)
{
	glDeleteTextures(1, &textures[tex_ind]->id_tex);

	if (!textures[tex_ind]->deleted)
	{
		for(int i = 0; i < textures[tex_ind]->referenced_mats.size(); i++)
		{ 
			for (int j = 0; j < textures[tex_ind]->referenced_mats[i]->textures.size(); j++)
			{ 
				if (textures[tex_ind]->referenced_mats[i]->textures[j]->filename == textures[tex_ind]->filename)
				{
					textures[tex_ind]->referenced_mats[i]->PopTexture(j);
			
				}
			}
				
		}
			

		delete textures[tex_ind];
	}

	textures[tex_ind] = nullptr;

	PopTexture(tex_ind);
}

void ComponentMaterial::PopTexture(const int& tex_index)
{
	for (int i = tex_index + 1; i < textures.size(); i++)
		textures[i - 1] = textures[i];

	textures.pop_back();


	if (textures.size() > 0)
	{
		std::vector<Component*> cmp_meshes;
		cmp_meshes = parent->GetComponents(CT_Mesh);
		if (cmp_meshes.size() > 0)
		{
			for (int i = 0; i < cmp_meshes.size(); i++)
			{
				ComponentMesh* aux = cmp_meshes[i]->AsMesh();
				if (aux != nullptr && aux->Material_Ind >= textures.size())
					aux->Material_Ind = -1;
			}
		}
	}
}

void ComponentMaterial::CleanUp()
{
	for (int i = 0; i < textures.size(); i++)
		DestroyTexture(i);


	parent = nullptr;
}

Texture* ComponentMaterial::CheckTexRep(std::string name)
{
	Texture* ret = nullptr;

	GameObject* par = this->parent;

	while (par != NULL && ret == nullptr)
	{
		std::vector<Component*> cmp_mats;
		cmp_mats = parent->GetComponents(CT_Material);
		for (int i = 0; i < cmp_mats.size(); i++)
		{
			ComponentMaterial* aux = cmp_mats[i]->AsMaterial();
			if(aux != nullptr)
				ret = aux->CheckNameRep(name);
		}

		par = par->parent;
	}

	return ret;
}

Texture* ComponentMaterial::CheckNameRep(std::string name)
{
	Texture* ret = nullptr;

	for (int i = 0; i < this->textures.size(); i++)
	{
		if(name.substr(name.find_last_of("\\/") + 1) == textures[i]->filename)
			ret = textures[i];

		if (ret != nullptr)
			break;

		if(name.substr(name.find_last_of("\\/") + 1) == textures[i]->filename.substr(textures[i]->filename.find_last_of("\\/") + 1))
			ret = textures[i];

		if (ret != nullptr)
			break;

		if(name == textures[i]->filename)
			ret = textures[i];

		if (ret != nullptr)
			break;

		if(name == textures[i]->filename.substr(textures[i]->filename.find_last_of("\\/") + 1))
			ret = textures[i];
	}

	return ret;
}

void ComponentMaterial::Load(JSON_Value* scene, const char* file)
{

}

void ComponentMaterial::Save(JSON_Value* scene, const char* file)
{
	JSON_Object* curr = json_value_get_object(scene);

	std::string obj = std::to_string(this->parent->UUID) + ".component.";
	std::string set_val;

	set_val = obj + "type";
	json_object_dotset_number(curr, set_val.c_str(), type);

	set_val = obj + "mat_name";
	json_object_dotset_string(curr, set_val.c_str(), name.c_str());

	json_serialize_to_file(scene, file);
	scene = json_parse_file(file);

	App->importer->mat_i->ExportMat(this);
}
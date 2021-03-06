#include "Resource.h"
#include "ModuleResourceManager.h"
#include "Application.h"
#include "MeshImport.h"
#include "MaterialImport.h"
#include "ResourceMesh.h"
#include "ResourceMaterial.h"
#include "ResourceTexture.h"

void Resource::LoadToMem()
{
	switch (par->type)
	{
	case RT_Prefab:
		break;
	case RT_Mesh:
		mesh.ptr = App->importer->mesh_i->LoadMesh(par->file.c_str());
		break;
	case RT_Material:
		mat.ptr = App->importer->mat_i->LoadMat(par->file.c_str());
		break;
	case RT_Texture:
		texture.ptr = App->importer->mat_i->LoadTexture(par->file.c_str());
		break;
	default:
		break;
	}
}

void Resource::UnloadFromMem()
{
	switch (par->type)
	{
	case RT_Prefab:
		break;
	case RT_Mesh:
		mesh.ptr->UnloadMem();
		delete mesh.ptr;
		mesh.ptr = nullptr;
		break;
	case RT_Material:
		mat.ptr->UnloadMem();
		delete mat.ptr;
		mat.ptr = nullptr;
		break;
	case RT_Texture:
		texture.ptr->UnloadMem();
		delete texture.ptr;
		texture.ptr = nullptr;
		break;
	default:
		break;
	}
}

bool Resource::IsInUse()
{
	return (par->UseCount > 0);
}

bool Resource::IsLoaded()
{
	switch(par->type)
	{
	case RT_Prefab:
		break;
	case RT_Mesh:
		return (mesh.ptr != nullptr);
	case RT_Material:
		return (mat.ptr != nullptr);
	case RT_Texture:
		return (texture.ptr != nullptr);
	default:
		return false;
	}
}

//--------------------------------------

bool DGUID::TrueComp(const DGUID cmp_id) const
{
	bool ret = false;

	std::vector<uint> f = TrueSum();
	std::vector<uint> s = cmp_id.TrueSum();

	for (int i = 0; i < f.size(); i++)
	{
		if (f[i] != s[i])
		{
			ret = (f[i] > s[i]);
			break;
		}
	}


	return ret;
}

std::vector<uint> DGUID::TrueSum() const
{
	std::vector<uint> ret;
	for (int i = 0; i < 16; i++)
	{
		if (i == 0)
			ret.push_back(MD5ID[i] + 1000 * MD5ID[i + 1]);
		else
		{
			ret.push_back(MD5ID[i] + 1000 * MD5ID[i + 1] + 1000000 * MD5ID[i + 2]);
			i++;
		}

		i++;
	}

	return ret;
}

bool DGUID::operator==(DGUID cmp_id)
{
	std::vector<uint> f = TrueSum();
	std::vector<uint> s = cmp_id.TrueSum();

	for (int i = 0; i < f.size(); i++)
	{
		if (f[i] != s[i])
			return false;
	}

	return true;
}

bool DGUID::operator==(const DGUID cmp_id)const
{
	bool ret = false;
	for (int i = 0; i < 32; i++)
	{
		ret = MD5ID[i] == cmp_id.MD5ID[i];
		if (!ret)
			break;
	}
	if (ret)
		return !ret;
	else
	{
		return TrueComp(cmp_id);
	}

}

void DGUID::SetInvalid()
{
	for (int i = 0; i < 32; i++)
		MD5ID[i] = -52;
}

bool DGUID::CheckValidity()
{
	bool ret = true;

	for (int i = 0; i < 32 && ret; i++)
	{
		ret = MD5ID[i] != -52;
	}

	return ret;
}
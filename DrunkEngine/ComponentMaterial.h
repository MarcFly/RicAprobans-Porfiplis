#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_

#include <vector>
#include <string>
#include "Color.h"
#include "Component.h"
#include "Globals.h"

class GameObject;
struct ResourceTexture;
struct ResourceMaterial;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial() { SetBaseVals(); };
	ComponentMaterial(GameObject* par);

	~ComponentMaterial() {};

	void PopTexture(const int& tex_index);

	void CleanUp();

	void DrawTextures(ResourceMesh* r_mesh);
	void DrawDiffuse(ResourceMesh* r_mesh, ResourceTexture* tex);

	void Load(const JSON_Object* comp);
	void Save(JSON_Array* comps);

public:

	ResourceMaterial* r_mat = nullptr;
	std::vector<ResourceTexture*> textures;

	// Non Specific Resource Vals
	bool AlphaTest;
	float AlphaVal;

public:
	void SetBaseVals()
	{
		this->type = CT_Material;

		this->multiple = true;

		this->parent = nullptr;

		this->to_pop = false;

		AlphaTest = false;
		float AlphaVal = 1;
	}
};

#endif
#ifndef _COMPONENT_MATERIAL_
#define _COMPONENT_MATERIAL_

#include <vector>
#include <string>
#include "GLEW/include/GL/glew.h"
#include "Color.h"
#include "Component.h"

class GameObject;
enum TextureMode
{
	TM_Error = -1,
	TM_DIFFUSE,
	TM_AMBIENT,

	TM_MAX
};

class ComponentMaterial;

struct Texture
{
	GLuint id_tex = 0;
	GLuint width, height;
	std::string filename;
	TextureMode type = TM_Error;
	ComponentMaterial* mparent;

	std::vector<ComponentMaterial*> referenced_mats;

	bool deleted = false;
};

class ComponentMaterial : Component
{
public:
	ComponentMaterial() {};
	ComponentMaterial(GameObject* par);

	~ComponentMaterial() {};

	void DestroyTexture(const int& tex_index);
	void PopTexture(const int& tex_index);

	void CleanUp();

	Texture* CheckTexRep(std::string name);
	Texture* CheckNameRep(std::string name);

public:

	uint NumDiffTextures = 0;
	std::vector<Texture*> textures;
	unsigned int NumProperties = 0;
	Color default_print = {1,1,1,1};

	GameObject* parent = nullptr;

	bool to_pop = false;

public:
	void SetBaseVals()
	{
		type = CT_Material;
		multiple = true;

		NumDiffTextures = 0;
		NumProperties = 0;
		default_print = { 1,1,1,1 };

		parent = nullptr;

		to_pop = false;
	}
};

#endif
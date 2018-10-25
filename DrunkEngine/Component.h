#ifndef _COMPONENT_
#define _COMPONENT_

#include "parson/parson.h"

class GameObject;

enum CTypes
{
	CT_Error = -1,
	CT_Mesh,
	CT_Material,
	CT_Camera,
	CT_Transform,

	CT_Max
};

class Component
{
public:
	Component(CTypes id_t) : type{ id_t } {};
	Component(CTypes id_t, GameObject* par) : type{ id_t }, parent{ par } {};
	~Component() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void PreUpdate() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void PostUpdate() {};
	virtual void CleanUp() {};

	virtual void Load(JSON_Object* root_value) {};
	virtual void Save(JSON_Object* root_value) {};

	virtual void SetBaseVals()
	{
		type = CT_Error;
		multiple = false;
	}

public:
	
	CTypes type;
	bool multiple;
	GameObject* parent;

public:
	
	

};

#endif
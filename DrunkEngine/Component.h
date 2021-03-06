#ifndef _COMPONENT_
#define _COMPONENT_

#include "parson/parson.h"
#include <string>
#include "ModuleResourceManager.h"

class GameObject;
class ComponentMaterial;
class ComponentMesh;
class ComponentCamera;
class ComponentTransform;
class ComponentBillboard;
class ComponentSkeleton;
class ComponentAnimation;

enum CTypes
{
	CT_Error = -1,
	CT_Mesh,
	CT_Material,
	CT_Camera,
	CT_Transform,
	CT_Billboard,
	CT_Skeleton,
	CT_Animation,

	CT_Max
};

class Component
{
public:
	Component() {};
	Component(CTypes id_t) : type{ id_t } {};
	Component(CTypes id_t, GameObject* par) : type{ id_t }, parent{ par } {};
	~Component() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void PreUpdate() {};
	virtual void Update(const float dt) {};
 	virtual void Draw() {};
	virtual void PostUpdate() {};
	virtual void CleanUp() {};

	virtual void Load(const JSON_Object* comp) {};
	virtual void Save(JSON_Array* comps) {};

	virtual void SetBaseVals()
	{
		type = CT_Error;
		multiple = false;
	}

public:
	bool active = true;
	DGUID UID;
	CTypes type;
	bool multiple;
	GameObject* parent;
	unsigned int count_number;
	bool to_pop = false;

	std::string name; // Save objects will have file name as name

public:
	// AsX Functions for non hardcoded acces to different type when it is
	ComponentMesh* AsMesh() {
		if (type == CT_Mesh)
			return (ComponentMesh*)this;
		
		return nullptr;
	}

	ComponentMaterial* AsMaterial() {
		if (type == CT_Material)
			return (ComponentMaterial*)this;

		return nullptr;
	}

	ComponentTransform* AsTransform() {
		if (type == CT_Transform)
			return (ComponentTransform*)this;

		return nullptr;
	}

	ComponentCamera* AsCamera() {
		if (type == CT_Camera)
			return (ComponentCamera*)this;

		return nullptr;
	}

	ComponentBillboard* AsBillboard() {
		if (type == CT_Billboard)
			return (ComponentBillboard*)this;

		return nullptr;
	}

	ComponentSkeleton* AsSkeleton() {
		if (type == CT_Skeleton)
			return (ComponentSkeleton*)this;

		return nullptr;
	}
	
	ComponentAnimation* AsAnimation() {
		if (type = CT_Animation)
			return (ComponentAnimation*)this;

		return nullptr;
	}

};

#endif
#include "ComponentSkeleton.h"
#include "Application.h"
#include "ConsoleWindow.h"
#include "SceneViewerWindow.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceSkeleton.h"
#include "FileHelpers.h"
#include "ResourceMesh.h"

ComponentSkeleton::ComponentSkeleton()
{
	SetBaseVals();
}

ComponentSkeleton::ComponentSkeleton(GameObject* par)
{
	SetBaseVals();

	parent = par;
};

void ComponentSkeleton::Update(const float dt)
{
	if (check_vecs < 2)
	{
		initial_pos = parent->GetTransform()->global_transform.Col3(3);
		initial_rot = parent->GetTransform()->GetRotFromMat(parent->GetTransform()->global_transform);
		initial_scale.x = parent->GetTransform()->global_transform[0][0];
		initial_scale.y = parent->GetTransform()->global_transform[1][1];
		initial_scale.z = parent->GetTransform()->global_transform[2][2];

		check_vecs++;
	}

	UpdateTransform();


}

void ComponentSkeleton::Draw()
{
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.1f, 0.0f);
	for (int i = 0; i < r_skel->bones.size(); i++)
		DrawToChildren(r_skel->bones[i]);

	glColor3f(0, 1, 0);

	if (check_vecs == 2)
		DrawDeformedMesh();


	glEnd();
}

void ComponentSkeleton::DrawDeformedMesh()
{
	c_mesh->deformable_mesh->SetValsFromMesh(c_mesh->r_mesh);
	
	//float3 par_pos = parent->GetTransform()->position;
	//for (int i = 0; i < c_mesh->deformable_mesh->num_vertex; ++i)
	//{
	//	c_mesh->deformable_mesh->vertex[i * 3] -= par_pos.x;
	//	c_mesh->deformable_mesh->vertex[i * 3 + 1] -= par_pos.y;
	//	c_mesh->deformable_mesh->vertex[i * 3 + 2] -= par_pos.z;
	//}

	for (int i = 0; i < c_mesh->r_mesh->num_vertex; i++)
		sums.push_back(0);

	DeformMesh(r_skel->bones);

	sums.clear();
}

void ComponentSkeleton::DeformMesh(std::vector<Bone*>& bones)
{
	ResourceMesh* d_mesh = c_mesh->deformable_mesh;
	ResourceMesh* mesh = c_mesh->r_mesh;

	for (int i = 0; i < bones.size(); ++i)
	{
		float4x4 b_trans = parent->GetTransform()->global_transform.Inverted();
			b_trans = b_trans * bones[i]->transform.global_transform;
		b_trans = b_trans * bones[i]->OffsetMatrix;

		for (int j = 0; j < bones[i]->weights.size(); ++j)
		{
			sums[bones[i]->weights[j]->VertexID] += bones[i]->weights[j]->w;
			// Vertex
			GLfloat* curr_vertex = &mesh->vertex[bones[i]->weights[j]->VertexID * 3];
			GLfloat* def_vertex = &d_mesh->vertex[bones[i]->weights[j]->VertexID * 3];

			float3 movement(curr_vertex[0], curr_vertex[1], curr_vertex[2]);

			float3 new_movement = b_trans.TransformPos(movement);
			
			def_vertex[0] += new_movement.x * bones[i]->weights[j]->w;
			def_vertex[1] += new_movement.y * bones[i]->weights[j]->w;
			def_vertex[2] += new_movement.z * bones[i]->weights[j]->w;

			// Normal
			GLfloat* curr_normal = &mesh->vert_normals[bones[i]->weights[j]->VertexID * 3];
			GLfloat* def_normal = &d_mesh->vert_normals[bones[i]->weights[j]->VertexID * 3];

			movement = float3(curr_normal[0], curr_normal[1], curr_normal[2]);

			new_movement = b_trans.TransformPos(movement);

			def_normal[0] += new_movement.x * bones[i]->weights[j]->w;
			def_normal[1] += new_movement.y * bones[i]->weights[j]->w;
			def_normal[2] += new_movement.z * bones[i]->weights[j]->w;
		}

		if (bones[i]->children.size() != 0)
			DeformMesh(bones[i]->children);
	}
}

void ComponentSkeleton::DrawToChildren(Bone* bone)
{
	for (int i = 0; i < bone->children.size(); i++)
	{
		if (bone->parent != nullptr)
		{
			glVertex3f(bone->transform.global_pos.x, bone->transform.global_pos.y, bone->transform.global_pos.z);
			glVertex3f(bone->children[i]->transform.global_pos.x, bone->children[i]->transform.global_pos.y, bone->children[i]->transform.global_pos.z);
		}

		DrawToChildren(bone->children[i]);
	}
}

void ComponentSkeleton::CleanUp()
{
	App->resources->Unused(UID);
	r_skel = nullptr;
	c_mesh = nullptr;

	//delete deformable_mesh;
	//deformable_mesh = nullptr;
}

void ComponentSkeleton::Load(const JSON_Object* comp)
{
	this->name = json_object_get_string(comp, "filename");
	this->UID = DGUID(name.c_str());


	if (!App->resources->InLibrary(UID))
		this->UID = App->resources->AddResource(name.c_str());
	if (App->resources->InLibrary(UID))
		App->importer->skel_i->LinkSkeleton(UID, this);

	CreateDeformableMesh();
}

void ComponentSkeleton::Save(JSON_Array* comps)
{
	JSON_Value* append = json_value_init_object();
	JSON_Object* curr = json_value_get_object(append);

	json_object_dotset_number(curr, "properties.type", type);

	json_object_dotset_string(curr, "properties.filename", App->resources->Library.at(UID)->file.c_str());

	json_array_append_value(comps, append);

}

float4x4 ComponentSkeleton::RecursiveParentInverted(float4x4& t, Bone& b)
{
	if (b.parent != nullptr)
	{
		//t = t * b.parent->OffsetMatrix.Inverted();
		t = t * b.parent->transform.global_transform.Inverted();
		t = RecursiveParentInverted(t, *b.parent);
	}
	return t;
}

void ComponentSkeleton::UpdateTransform()
{
	ComponentTransform* root_transform = &r_skel->bones[0]->transform;
	ComponentTransform* parent_transform = parent->GetTransform();

	root_transform->global_pos = parent_transform->global_pos - initial_pos;

	root_transform->global_rot.x = parent_transform->global_rot.x;
	root_transform->global_rot.y = parent_transform->global_rot.y;
	root_transform->global_rot.z = parent_transform->global_rot.z;
	root_transform->global_rot.w = parent_transform->global_rot.w;

	root_transform->global_scale = float3(parent->GetTransform()->global_scale.x / initial_scale.x, parent->GetTransform()->global_scale.y / initial_scale.y, parent->GetTransform()->global_scale.z / initial_scale.z);
	//root_transform->global_scale = float3(1, 1, 1);

	root_transform->global_transform = float4x4::FromTRS(root_transform->global_pos, root_transform->global_rot, root_transform->global_scale);

	r_skel->CalculateSkeletonTransforms();
}

bool CkeckSkeletonValidity()
{
	return true;
}

/*
void ComponentSkeleton::LinkMesh()
{
		
}*/

void ComponentSkeleton::CreateDeformableMesh()
{
	if (c_mesh == nullptr)
		for (int i = 0; i < parent->components.size(); i++)
		{
			if (parent->components[i]->type == CT_Mesh)
			{
				c_mesh = parent->components[i]->AsMesh();
				break;
			}
		}

	c_mesh->deformable_mesh = new ResourceMesh(c_mesh->r_mesh);


}

#include "SkeletonImport.h"
#include "Resource.h"
#include "Color.h"
#include "ComponentTransform.h"

struct DGUID;

struct BoneWeight
{
	uint VertexID = -1;
	float w = 0;
};

struct Bone
{
	Bone() {};

	bool active = false;
	DGUID ID;
	std::string name = "Unknown_Bone";
	std::string affected_mesh = "UnknownMesh";
	std::vector<BoneWeight*> weights;
	ComponentTransform transform;

	float4x4 OffsetMatrix;

	float4x4 last_anim_step = float4x4::identity;

	float3 permanent_local_pos = float3::zero;
	Quat permanent_local_rot = Quat::identity;
	float3 permanent_local_scale = float3::one;

	std::vector<Bone*> children;

	uint fast_id = 0;
	uint fast_par_id = 0;
	Bone* parent = nullptr;

	void OrderBones();
	void AdjustChildren(const int& i);
	Bone*  GetChild(const uint& par_id);
	void CalculateBoneGlobalTransforms();
	Bone* FindBone(const std::string &bonename);

	~Bone()
	{
		for(int i = 0; i < weights.size(); i++)
		{
			delete weights[i];
			weights[i] = nullptr;
		}
		weights.clear();
		name.clear();	
	}
};

struct ResourceSkeleton
{
	std::vector<Bone*> bones;

	void OrderBones();
	void AdjustChildren(const int& i);
	Bone*  GetChild(const uint& par_id);
	Bone* FindBone(const std::string &bonename);
	void CalculateSkeletonTransforms();

	void UnloadMem();
	~ResourceSkeleton();
};

class MetaSkeleton : public MetaResource
{
public:
	MetaSkeleton() { type = RT_Skeleton; };
	~MetaSkeleton() {};

	DGUID origin_mesh;
	
	void LoadMetaFile(const char* file);
};
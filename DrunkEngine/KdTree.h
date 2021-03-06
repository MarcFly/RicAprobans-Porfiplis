#ifndef _KDTree_H_
#define _KDTree_H_

#include <vector>
#include "Globals.h"

class GameObject;
class ComponentCamera;

class KDTree
{
public:
	class Node
	{
	public:
		enum Axis
		{
			Axis_X,
			Axis_Y,
			Axis_Z,
			Axis_NULL
		};

	public:
		Node(std::vector<GameObject*>& objs_in_node, Node * parent, KDTree * root);
		Node(std::vector<GameObject*>& objs_in_node, Node * parent, AABB bounding_box);
		~Node();

		void Update();
		void Draw();
		void CleanUp();

		void SetNodeVertex();

		void CreateNodes();
		std::vector<GameObject*> GetObjectsInNode(const AABB& new_bounding_box);

		void SetVertexPos(const vec& min, const vec& max);

		float GetKdTreeCut(const Axis axis);

		std::vector<GameObject*> GetObjsInNode(const Node * node);

		bool CheckNodeRepeat(const AABB new_bb);
		bool CheckMeshesColliding() const;

	public:
		int id;
		int subdivision;

		KDTree * root = nullptr;
		Node * parent = nullptr;
		std::vector<Node*> child;

		Axis axis_to_check = Axis::Axis_NULL;

		std::vector<GameObject*> objects_in_node;
		AABB bounding_box;

	};

public:
	KDTree(int elements_per_node, int max_subdivisions);
	~KDTree();

	void Update();
	void CleanUp();

	void RecursiveGetStaticObjs(const GameObject * obj);
	void CheckKDTreeInsideFrustum(const Node * node, const ComponentCamera * cam);

public:
	int elements_per_node;
	int max_subdivisions;

	bool to_draw;

	Node * base_node = nullptr;
	std::vector<Node*> nodes;
	std::vector<GameObject*> static_objs;
};


#endif

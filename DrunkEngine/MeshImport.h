#ifndef _MESH_IMPORT_
#define _MESH_IMPORT_

#include "Assimp/include/scene.h"
#include "GLEW/include/GL/glew.h"
#include <vector>

class ComponentMesh;
class GameObject;
struct ResourceMesh;
struct DGUID;
class MetaMesh;

class MeshImport {
public:
	MeshImport() {};
	~MeshImport() {};

	void Init();

	void LinkMesh(DGUID fID, ComponentMesh* mesh);
	ResourceMesh* LoadMesh(const char* file);

	void ExportAIMesh(const aiMesh* mesh, const int& mesh_id, const char* path = nullptr);
	
	void ExportIndexNormals(const int& ind, std::vector<GLfloat>& normals, std::vector<GLuint>& index, std::vector<GLfloat>& vertex);

	void ExportMeta(const aiMesh* mesh, const int& mesh_id, std::string path);
	void LoadMeta(const char* file, MetaMesh* meta);

};

#endif
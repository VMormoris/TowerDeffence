#include "../../engpch.h"
#include "MeshManager.h"

namespace Engine {

	std::vector<std::string> MeshManager::mesh_names;
	std::vector<Mesh*> MeshManager::meshes;
	Loader MeshManager::loader;

	int MeshManager::findMesh(const char* filename) {
		for (int i = 0; i < mesh_names.size();i++) {
			std::string name = mesh_names[i];
			if (name.compare(filename) == 0) return i;
		}
		return -1;
	}


	void MeshManager::Clear(void) {
		for (Mesh* mesh : meshes) delete mesh;
	}

	Mesh* MeshManager::RequestMesh(const char* filename) {
		int index = findMesh(filename);
		if (index != -1) return meshes[index];
		Mesh* mesh =loader.load(filename);
		if (mesh != nullptr) {
			mesh->Init();
			mesh->Delete();
			mesh_names.push_back(std::string(filename));
			meshes.push_back(mesh);
		}
		return mesh;
	}

}
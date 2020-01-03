#pragma once
#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include "../Lab/Loader.h"
namespace Engine {

	class ENGINE_API MeshManager {
	protected:
		static int findMesh(const char* filename);
		static std::vector<Mesh*> meshes;
		static std::vector<std::string> mesh_names;
		static Loader loader;
	public:
		static void Clear(void);
		static Mesh* RequestMesh(const char* filename);
	};

}

#endif // !MESH_MANAGER_H

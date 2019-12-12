#pragma once

//#include "utils.h"
#include "Mesh.h"

namespace Engine {

	/**
	* Load Mesh from .obj file
	* @param filename path to a .obj file
	* @return A pointer to Mesh object
	* @see Mesh.h
	*/
	ENGINE_API Mesh* loadObj(const char* filename);

}
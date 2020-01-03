#pragma once
#ifndef FIELD_H
#define FIELD_H
#include <Engine.h>




class Field : public Engine::GameObject {
public:
	Field(void) {
		Engine::Object3D* geometry = new Engine::Object3D("../Assets/Terrain/terrain.obj");
		objects.push_back(geometry);
		center = glm::vec3(18.f, 0, 18.f);
		scale = glm::vec3(20.f, 1.f, 20.f);
		//scale = glm::vec3(1.f);
		angle = glm::radians(0.f);
		rotation_vector = glm::vec3(0, 1, 0);
	}
};


#endif // !FIELD_H

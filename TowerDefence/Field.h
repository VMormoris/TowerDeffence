#pragma once
#ifndef FIELD_H
#define FIELD_H
#include <Engine.h>

class Field : public Engine::GameObject {
public:
	Field(void) {
		Engine::Object3D* geometry = new Engine::Object3D("../Data/Knossos/knossos.obj");
		objects.push_back(geometry);
		center = glm::vec3(0, 0, 0);
		scale = glm::vec3(0.25f);
		angle = glm::radians(0.f);
		rotation_vector = glm::vec3(0, 1, 0);
	}
};


#endif // !FIELD_H

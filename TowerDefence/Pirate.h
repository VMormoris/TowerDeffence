#pragma once
#ifndef PIRATE_H
#define PIRATE_H


#include <Engine.h>


class Pirate : public Engine::GameObject {
public:
	Pirate(void) {
		Engine::Object3D* geometry = new Engine::Object3D("../Data/Pirates/skeleton.obj");
		objects.push_back(geometry);
		center = glm::vec3(0, 1, 0);
		angle = glm::radians(10.f);
		rotation_vector = glm::vec3(0, 0, 1);
		scale = glm::vec3(0.07f);
	}

	bool onClick(const SDL_Event& event) {
		std::cout << "Clicked" << std::endl;
		return true;
	}
};


#endif // !PIRATE_H

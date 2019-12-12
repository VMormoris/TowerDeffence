#include "Pirate.h"
#include "gtc/matrix_transform.hpp"


Pirate::Pirate() {

	mesh = Engine::loadObj("../Data/Pirates/skeleton.obj");
	mesh->setupBuffers();
	transformation_matrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.f), glm::radians(10.f), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0), glm::vec3(0.07f));
	normal_matrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(transformation_matrix))));
}

Pirate::~Pirate() { delete mesh; }

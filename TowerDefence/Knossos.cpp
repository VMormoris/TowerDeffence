#include "Knossos.h"
#include "gtc/matrix_transform.hpp"

Knossos::Knossos(void) {
	mesh= Engine::loadObj("../Data/Knossos/knossos.obj");
	mesh->setupBuffers();
	transformation_matrix = glm::scale(glm::mat4(1.f), glm::vec3(0.25f));
	normal_matrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(transformation_matrix))));
}

Knossos::~Knossos(void) { delete mesh; }
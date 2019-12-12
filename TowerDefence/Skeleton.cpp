#include "Skeleton.h"
#include "gtc/matrix_transform.hpp"


Skeleton::Skeleton() {
	mesh = Engine::loadObj("../Data/Pirates/weight.obj");
	mesh->setupBuffers();
}

Skeleton::~Skeleton() { delete mesh; }

void Skeleton::Update(float time) {
	glm::mat4 object_translation = glm::translate(glm::mat4(1.0), glm::vec3(2, 5, 0));
	glm::mat4 object_rotation = glm::rotate(glm::mat4(1.0), 1.5f * time, glm::vec3(0, 1, 0));
	glm::mat4 object_scale = glm::scale(glm::mat4(1.0), glm::vec3(0.8f));
	transformation_matrix = glm::translate(glm::mat4(1.0), glm::vec3(-4, 2, -2)) * object_translation * object_rotation * object_scale;
	normal_matrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(transformation_matrix))));
}

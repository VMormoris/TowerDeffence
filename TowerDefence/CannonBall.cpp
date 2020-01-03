#include "CannonBall.h"

CannonBall::CannonBall(glm::vec3 start, glm::vec3 target): Engine::GameObject() {
	tag = "cannonball";
	objects.push_back(new Ball());
	center = start;
	angle = glm::radians(0.f);
	rotation_vector = glm::vec3(0, 1.f, 0.f);
	scale = glm::vec3(0.1f);
	glm::vec3 direction = glm::normalize(target - start);
	float distance = glm::distance(target, start);
	SPEED = direction * distance;
}

bool CannonBall::onCollision(CollisionListener* other) {
	std::cout << "Collision decteted" << std::endl;
	glm::vec3 temp = SPEED;
	SPEED = ((CannonBall*)other)->SPEED;
	((CannonBall*)other)->SPEED = temp;
	return true;
}


void CannonBall::Update(float dt) {
	center += SPEED * dt;
}
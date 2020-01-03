#include "Tower.h"

Tower::Tower(void) : Engine::GameObject() {
	last_fire = -1.0f;
	objects.push_back(new TowerGeometry());
	center = glm::vec3(0.f, 0.f, 16.f);
	angle = glm::radians(0.f);
	rotation_vector = glm::vec3(0.f, 1.f, 0.f);
	scale = glm::vec3(0.4f);
}


CannonBall* Tower::Fire(glm::vec3 target, float time) {
	if (time >= last_fire + cooldown) {
		last_fire = time;
		Engine::AudioManager::RequestSound("../Assets/Sounds/CannonFire.wav")->Play();
		return new CannonBall(center+(glm::vec3(-0.0101f, 9.5f, -0.0758f)*scale), target);
	}
	else
		return NULL;
}

glm::vec3 Tower::getCoords(void) { return center; }
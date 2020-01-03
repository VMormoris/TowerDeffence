#pragma once

#ifndef TOWER_H
#define TOWER_H

#include "CannonBall.h"

class TowerGeometry: public Engine::Object3D{
public:
	TowerGeometry(void) :Object3D("../Assets/MedievalTower/tower.obj") {}
};

class Tower :public Engine::GameObject {
private:
	const float cooldown = 2.f;
	float last_fire;
public:
	Tower(void);
	CannonBall* Fire(glm::vec3 target, float time);
	glm::vec3 getCoords(void);
};

#endif//!TOWER_H
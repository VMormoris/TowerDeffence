#pragma once
#ifndef CANNON_BALL_H
#define CANNON_BALL_H

#include <Engine.h>
#include "CollisionListener.h"

	
class Ball : public Engine::Object3D {
public:
	Ball(void) : Object3D("../Assets/Various/cannonball.obj") {}
};

class CannonBall : public Engine::GameObject, public CollisionListener {
public:
	glm::vec3 SPEED;
	CannonBall(glm::vec3 start, glm::vec3 target);
	bool onCollision(CollisionListener* other) override;
	void Update(float dt) override;
	glm::vec3 getCoords(void) { return center; }
};

#endif // !CANNON_BALL_H

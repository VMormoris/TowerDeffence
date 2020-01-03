#pragma once
#ifndef PIRATE_H
#define PIRATE_H


#include <Engine.h>
#include "CollisionListener.h"
class Body : public Engine::Object3D{
public:
	Body(void) : Object3D("../Assets/Pirate/pirate_body.obj") {}
};

class LeftFoot : public Engine::Object3D {
public:
	LeftFoot(void) : Object3D("../Assets/Pirate/pirate_left_foot.obj") {}
	void Update(float time) override;
};

class RightFoot : public Engine::Object3D {
public:
	RightFoot(void) : Object3D("../Assets/Pirate/pirate_left_foot.obj") {}
	void Update(float time) override;
};

class Arm : public Engine::Object3D {
public:
	Arm(void) : Object3D("../Assets/Pirate/pirate_arm.obj") {}
	void Update(float time) override;
};

class MaxLife : public Engine::Object3D {
public:
	MaxLife(void) : Object3D("../Assets/Various/plane_red.obj") {}
};

class Life : public Engine::Object3D {
public:
	Life(void) : Object3D("../Assets/Various/plane_green.obj") {}
};


class Pirate : public Engine::GameObject, public CollisionListener {
private:
	int map_index;
	float timestamp;
	float change_index;
	glm::vec3 direction;
	const float MAX_LIFE = 100.f;
	float life;
	glm::vec3 start_pos;
	float start_angle;
	const float RADIUS = 12.87075f;
	const glm::vec3 CENTER=glm::vec3(-0.5957f, 11.683f, -4.274f);
public:
	Pirate(void);
	void Update(float dt) override;
	void DrawShadowmap(Engine::Shader& program) override;
	void Draw(Engine::Shader& program) override;
	glm::vec3 CenterAt(float time);
	float AngleAt(float time);
	float getRadius(void);
	glm::vec3 getCoords(void);
	bool onCollision(CollisionListener* other) override;
	bool onClick(const SDL_Event& event) override { std::cout << "Pirate was clicked!" << std::endl; return true; }
	glm::vec3* getBoxes(void);
};


#endif // !PIRATE_H

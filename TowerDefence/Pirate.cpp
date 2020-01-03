#include "Pirate.h"
#include "Road.h"
#include <cmath>
#include "CannonBall.h"

bool Pirate::onCollision(CollisionListener* other) {
	life -= 20.f;
	if (life <= 0.0f) DeleteASAP();
	return true;
}

void Arm::Update(float time) { unique_rotation = glm::sin(2.f * time); }

void LeftFoot::Update(float time) { center_pivot.z = -2.f + (6.f * glm::sin(time)); }

void RightFoot::Update(float time) { center_pivot.z = -2.f + (6.f * -glm::sin(time)); }

glm::vec3 Pirate::getCoords(void){
	return center + (glm::rotate(CENTER, angle, rotation_vector) * scale);
}


void Pirate::Update(float dt) {

	timestamp += dt;
	
	center = CenterAt(timestamp);
	angle = AngleAt(timestamp);
	
	for (Engine::Object3D* obj : objects) obj->Update(timestamp);
}




void Pirate::DrawShadowmap(Engine::Shader& program) {
	for (int i = 0; i < objects.size() - 2; i++) objects[i]->DrawShadowmap(program, angle, center, rotation_vector, scale);
}

void Pirate::Draw(Engine::Shader& program) {
	for (int i = 0; i < objects.size() - 2; i++) objects[i]->Draw(program, angle, center, rotation_vector, scale);
	glm::vec3 newCenter = glm::vec3(center.x, center.y + 3.f, center.z);
	glm::vec3 newScale = glm::vec3(1.f, 1.f, 0.1f);
	//float newAngle = glm::radians(-90.f);
	//glm::vec3 newRotationVector = glm::vec3(1.f, 0.f, 0.f);
	objects[objects.size() - 2]->Draw(program, angle, newCenter, rotation_vector, newScale);
	newScale = glm::vec3(1.f * (life / MAX_LIFE), 1.f, 0.1f);
	newCenter = glm::vec3(center.x  , center.y + 3.001f, center.z);
	objects[objects.size() - 1]->Draw(program, angle, newCenter, rotation_vector, newScale);
}

float Pirate::getRadius(void) {
	return RADIUS * 0.09;
}


Pirate::Pirate(void): Engine::GameObject() {
	tag = "pirate";
	Engine::Object3D* geometry = new Body();
	objects.push_back(geometry);
	geometry = new Arm();
	geometry->setExtra(glm::radians(0.f), glm::vec3(7.5f, 12.f, 0.f), glm::vec3(1, -1, 0));
	objects.push_back(geometry);
	geometry = new LeftFoot();
	geometry->setExtra(glm::radians(0.f), glm::vec3(-4, 0.5f, -2), glm::vec3(0.5, 0, 0));
	objects.push_back(geometry);
	geometry = new RightFoot();
	geometry->setExtra(glm::radians(0.f), glm::vec3(4, 0.5, -2), glm::vec3(1, 0, 0));
	objects.push_back(geometry);
	geometry = new MaxLife();
	objects.push_back(geometry);
	geometry = new Life();
	objects.push_back(geometry);
	center = start_pos =glm::vec3(0.f, 0.f, -1.f);
	angle = start_angle = glm::radians(180.f);
	rotation_vector = glm::vec3(0.f, 1.f, 0.f);
	scale = glm::vec3(0.09f);
	map_index = 0;
	timestamp = 0.f;
	change_index = 2.5f;
	life = 100.f;
}

glm::vec3* Pirate::getBoxes(void) {
	glm::vec3* data = new glm::vec3[8];
	for (int i = 0; i < 4; i++) {
		glm::vec3 vec1 = glm::rotate(objects[i]->getMinOBB(), angle, rotation_vector);
		glm::vec3 vec2 = glm::rotate(objects[i]->getMaxOBB(), angle, rotation_vector);
		glm::vec3 min(std::min(vec1.x, vec2.x), std::min(vec1.y, vec2.y), std::min(vec1.z, vec2.z));
		glm::vec3 max(std::max(vec1.x, vec2.x), std::max(vec1.y, vec2.y), std::max(vec1.z, vec2.z));
		data[i * 2] = center + (min * scale);
		data[i * 2 + 1] = center + (max * scale);
	}
	return data;
}


glm::vec3 Pirate::CenterAt(float time) {
	glm::vec3 speed;
	float timestamp = time;
	glm::vec3 center = start_pos;
	float dt;
	while (timestamp > 0) {
		if (timestamp <= 5.5){
			speed = glm::vec3(0.f, 0.f, 2.f);
			center =center + timestamp * speed;
			timestamp = 0;
			break;
		}
		else if (timestamp > 9.5f && timestamp <= 15.5f) {
			speed = glm::vec3(0.f, 0.f, 2.f);
			dt = timestamp - 9.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 7.5 ){
			speed = glm::vec3(1.f, 0.f, 1.f);
			dt = timestamp - 5.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 15.5f && timestamp <= 17.5) {
			speed = glm::vec3(1.f, 0.f, 1.f);
			dt = timestamp - 15.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 19.5f && timestamp <= 21.5) {
			speed = glm::vec3(1.f, 0.f, 1.f);
			dt = timestamp - 19.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 9.5f){
			speed = glm::vec3(1.f, 0.f, 1.f);
			dt = timestamp - 7.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 19.5) {
			speed = glm::vec3(1.f, 0.f, 1.f);
			dt = timestamp - 17.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 27.5f){
			speed = glm::vec3(2.f, 0.f, 0.f);
			dt = timestamp - 21.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 41.5f && timestamp <= 43.5f) {
			speed = glm::vec3(2.f, 0.f, 0.f);
			dt = timestamp - 41.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 29.5f){
			speed = glm::vec3(1.f, 0.f, -1.f);
			dt = timestamp - 27.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 31.5) {
			speed = glm::vec3(0.f, 0.f, -2.f);
			dt = timestamp - 29.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 33.5f && timestamp <= 35.5f) {
			speed = glm::vec3(1.f, 0.f, -1.f);
			dt = timestamp - 33.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 43.5f && timestamp <= 45.5f) {
			speed = glm::vec3(1.f, 0.f, -1.f);
			dt = timestamp - 43.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 45.5f && timestamp <= 47.5) {
			speed = glm::vec3(0.f, 0.f, -2.f);
			dt = timestamp - 45.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 31.5f && timestamp <= 33.5f) {
			speed = glm::vec3(1.f, 0.f, -1.f);
			dt = timestamp - 31.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 35.5f && timestamp <= 39.5) {
			speed = glm::vec3(0.f, 0.f, -2.f);
			dt = timestamp - 35.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if(timestamp > 39.5 && timestamp<=41.5){
			speed = glm::vec3(1.f, 0.f, -1.f);
			dt = timestamp - 39.5;
			center = center + dt * speed;
			timestamp -= dt;			
		}
		else if ((timestamp > 47.5) && (timestamp <= 49.5)) {
			speed = glm::vec3(-1.f, 0.f, -1.f);
			dt = timestamp - 47.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp > 49.5f && timestamp <= 53.5f) {
			speed = glm::vec3(-2.f, 0.f, 0.f);
			dt = timestamp - 49.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else if (timestamp <= 55.5f && timestamp > 53.5) {
			speed = glm::vec3(-1.f, 0.f, -1.f);
			dt = timestamp - 53.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
		else {
			speed = glm::vec3(0.f, 0.f, -2.f);
			dt = timestamp - 55.5;
			center = center + dt * speed;
			timestamp -= dt;
		}
	}
	return center;
}

float Pirate::AngleAt(float time) {
	float angular_speed = 0.f;
	float timestamp = time;
	float angle = start_angle;
	float dt;
	while (timestamp > 0) {
		if (timestamp <= 5.5) {
			angular_speed = 0.f;
			dt = timestamp;
			angle += angular_speed * dt;
			timestamp = 0;
			break;
		}
		else if (timestamp <= 7.5 ){
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 5.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 7.5 && timestamp <= 9.5) {
			angular_speed = glm::radians(-90.f) / 2;
			dt = timestamp - 7.5;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 9.5 && timestamp <= 15.5) {
			angular_speed = 0.f;
			dt = timestamp - 9.5;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 15.5 && timestamp <= 17.5) {
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 15.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 17.5 && timestamp <= 19.5) {
			angular_speed = glm::radians(-90.f) / 2;
			dt = timestamp - 17.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 19.5 && timestamp <= 21.5) {
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 19.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 21.5f && timestamp <= 27.5f) {
			angular_speed = 0;
			dt = timestamp - 21.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 27.5 && timestamp <= 29.5f) {
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 27.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 29.5 && timestamp <= 31.5f) {
			angular_speed = 0;
			dt = timestamp - 29.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp>31.5 && timestamp <=33.5 ) {
			angular_speed = glm::radians(-90.f) / 2;
			dt = timestamp - 31.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 33.5 && timestamp <= 35.5) {
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 33.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 35.5 && timestamp <= 39.5) {
			angular_speed = 0;
			dt = timestamp - 35.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 39.5 && timestamp <= 41.5) {
			angular_speed = glm::radians(-90.f) / 2;
			dt = timestamp - 39.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 41.5 && timestamp <= 43.5) {
			angular_speed = 0;
			dt = timestamp - 41.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 43.5 && timestamp <= 45.5) {
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 43.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 45.5 && timestamp <= 47.5) {
			angular_speed = 0;
			dt = timestamp - 45.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 47.5 && timestamp <= 49.5) {
			angular_speed = glm::radians(90.f) / 2;
			dt = timestamp - 47.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 49.5 && timestamp <= 53.5) {
			angular_speed = 0;
			dt = timestamp - 49.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else if (timestamp > 53.5 && timestamp <= 55.5) {
			angular_speed = glm::radians(-90.f) / 2;
			dt = timestamp - 53.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
		else {
			angular_speed = 0;
			dt = timestamp - 55.5f;
			angle += angular_speed * dt;
			timestamp -= dt;
		}
	}
	return angle;
}
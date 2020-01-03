#include "../engpch.h"
#include "Camera.h"

namespace Engine {

	Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upvec) {
		coordinates = position;
		target_coords = target;
		up_vector = upvec;
	}

	Camera::~Camera(void) {}


	void Camera::Update(float dt) {
		glm::vec3 direction = glm::normalize(target_coords - coordinates);

		//Move towards to the camera
		coordinates += movement.z * movement_speed * direction * dt;
		target_coords += movement.z * movement_speed * direction * dt;

		//Move sideways
		glm::vec3 right = glm::normalize(glm::cross(direction, up_vector));
		coordinates += movement.x * movement_speed * right * dt;
		target_coords += movement.x * movement_speed * right * dt;

		//Move up and down
		coordinates += movement.y * movement_speed * up_vector * dt;
		target_coords += movement.y * movement_speed * up_vector * dt;
		
		glm::mat4 rotation = glm::mat4(1.0f);
		float angular_speed = glm::pi<float>() * 0.0025f;

		rotation *= glm::rotate(glm::mat4(1.0), look_angle_destination.y * angular_speed, right);
		rotation *= glm::rotate(glm::mat4(1.0), -look_angle_destination.x * angular_speed, up_vector);
		look_angle_destination = glm::vec2(0);
		
		direction = rotation * glm::vec4(direction, 0);
		float dist = glm::distance(coordinates, target_coords);
		target_coords = coordinates + direction * dist;

		view_matrix = glm::lookAt(coordinates, target_coords, up_vector);
	}


	void Camera::MoveForward(bool enable) { movement.z = (enable) ? 10 : 0; }
	void Camera::MoveBackwards(bool enable) { movement.z = (enable) ? -10 : 0; }
	void Camera::MoveUpwards(bool enable) { movement.y = (enable) ? 1 : 0; }
	void Camera::MoveDownwards(bool enable) { movement.y = (enable) ? -1 : 0; }
	void Camera::MoveRight(bool enable) { movement.x = (enable) ? 1 : 0; }
	void Camera::MoveLeft(bool enable) { movement.x = (enable) ? -1 : 0; }
	void Camera::LookAt(glm::vec2 dcoords) { look_angle_destination = glm::vec2(1, -1) * dcoords; }


	void Camera::setMatrices(int width, int height) {
		projection_matrix = glm::perspective(glm::radians(60.f), width / (float)height, 0.1f, 1500.0f);
		view_matrix = glm::lookAt(coordinates, target_coords, up_vector);
	}

	glm::mat4 Camera::getProjectionMatrix(void) { return projection_matrix; }
	glm::mat4 Camera::getViewMatrix(void) { return view_matrix; }
	glm::vec3 Camera::getCoordinates(void) { return coordinates; }
}
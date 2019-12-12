#include "Camera.h"
#include "gtc/type_ptr.hpp"
#include "gtc/matrix_transform.hpp"

namespace Engine{

	Camera::Camera(void) {
		m_camera_position = glm::vec3(1, 3, -10);
		m_camera_target_position = glm::vec3(0, 0, 0);
		m_camera_up_vector = glm::vec3(0, 1, 0);
	}

	Camera::~Camera(void) {}

	void Camera::SetMatrixes(int width, int height) {
		m_projection_matrix = glm::perspective(glm::radians(60.f), width / (float)height, 0.1f, 1500.0f);
		m_view_matrix = glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector);
	}

	glm::mat4 Camera::GetViewMatrix(void) { return m_view_matrix; }

	glm::mat4 Camera::GetProjectionMatrix(void) { return m_projection_matrix; }

	void Camera::Update(float dt) {
		float movement_speed = 2.0f;
		glm::vec3 direction = glm::normalize(m_camera_target_position - m_camera_position);

		m_camera_position += m_camera_movement.x * movement_speed * direction * dt;
		m_camera_target_position += m_camera_movement.x * movement_speed * direction * dt;

		glm::vec3 right = glm::normalize(glm::cross(direction, m_camera_up_vector));
		m_camera_position += m_camera_movement.y * movement_speed * right * dt;
		m_camera_target_position += m_camera_movement.y * movement_speed * right * dt;

		glm::mat4 rotation = glm::mat4(1.0f);
		float angular_speed = glm::pi<float>() * 0.0025f;

		rotation *= glm::rotate(glm::mat4(1.0), m_camera_look_angle_destination.y * angular_speed, right);
		rotation *= glm::rotate(glm::mat4(1.0), -m_camera_look_angle_destination.x * angular_speed, m_camera_up_vector);
		m_camera_look_angle_destination = glm::vec2(0);

		direction = rotation * glm::vec4(direction, 0);
		float dist = glm::distance(m_camera_position, m_camera_target_position);
		m_camera_target_position = m_camera_position + direction * dist;

		m_view_matrix = glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector);
	}




}
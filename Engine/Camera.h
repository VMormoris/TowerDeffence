#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Core.h"
#include "glm.hpp"

namespace Engine {

	class ENGINE_API Camera{
	private:

		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;
		glm::vec3 m_camera_position;
		glm::vec3 m_camera_target_position;
		glm::vec3 m_camera_up_vector;
		glm::vec2 m_camera_movement;
		glm::vec2 m_camera_look_angle_destination;

	public:

		Camera(void);
		~Camera(void);

		void Update(float dt);
		
		void SetMatrixes(int width, int height);

		glm::mat4 GetProjectionMatrix(void);
		glm::mat4 GetViewMatrix(void);
		glm::vec3 GetCameraCoords(void) { return m_camera_position; }

	};

}


#endif // !CAMERA_H

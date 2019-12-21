#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "engpch.h"
#include "Core.h"

namespace Engine {

	class ENGINE_API Camera {
	private:
		/**
		* Camera's view matrix
		*/
		glm::mat4 view_matrix;
		/**
		* Camera's projection matrix
		*/
		glm::mat4 projection_matrix;
		/**
		* Camera's coordinates in 3D space
		*/
		glm::vec3 coordinates;
		/**
		* Camera's target coordinates in 3D space
		*/
		glm::vec3 target_coords;
		/**
		* Camera's Up Vector
		*/
		glm::vec3 up_vector;
		/**
		* Camera's representing the move of the camera in each of the three axes
		*/
		glm::vec3 movement;
		/**
		* Look angle destination
		*/
		glm::vec2 look_angle_destination;
		float movement_speed = 2.0f;
	public:
		Camera(void) {}
		Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upvec=glm::vec3(0,1,0));
		~Camera(void);

		
		/**
		* Update camera position and viewing angle
		* @param dt Time pass since last Render call
		*/
		void Update(float dt);

		/**
		* Enable or disable movent Upwards
		* @param enable ...
		*/
		void MoveUpwards(bool enable);
		/**
		* Enable or disable movent Upwards
		* @param enable ...
		*/
		void MoveDownwards(bool enable);
		/**
		* Enable or disable movent Upwards
		* @param enable ...
		*/
		void MoveRight(bool enable);
		/**
		* Enable or disable movent Upwards
		* @param enable ...
		*/
		void MoveLeft(bool enable);
		/**
		* Enable or disable movent Upwards
		* @param enable ...
		*/
		void MoveForward(bool enable);
		/**
		* Enable or disable movent Upwards
		* @param enable ...
		*/
		void MoveBackwards(bool enable);

		/**
		* Change viewing angle of camera
		* @param dcoords dx, dy of Mouse
		*/
		void LookAt(glm::vec2 dcoords);
		
		/**
		* Create Projection and view matrix
		* @param width Width of the window
		* @param height Height of the window
		*/
		void setMatrices(int width, int height);

		/**
		* Getter for Camera's position
		*/
		glm::vec3 getCoordinates(void);
		/**
		* Getter for Camera's projection matrix
		*/
		glm::mat4 getProjectionMatrix(void);
		/**
		* Getter for Camera view matrix
		*/
		glm::mat4 getViewMatrix(void);
	};
	
}

#endif // !CAMERA_H

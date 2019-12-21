#pragma once
#ifndef OBJECT_3D
#define OBJECT_3D
#include "engpch.h"
#include "Shader.h"
#include "Mesh.h"


namespace Engine {


	/* ***** Extra Info *****
		An Human in our 3D world my be consisting multiple Object3D
			For example on for the head, one for the body, one for each hand and one for each foot
			The human in our 3D world can move it's part by separately in order to for animation to be emulated
	*/


	/**
	* Class for Drawing and moving a Mesh into the World of our Game.
	*/
	class ENGINE_API Object3D{
	protected:
		/**
		* Pivot from center of GameObject
		*/
		glm::vec3 center_pivot;
		/**
		* Angle for unique rotation of the our Mesh
		*/
		float unique_rotation;
		/**
		* Vector usefull for apllying rotation
		*/
		glm::vec3 rotation_vector;
		/**
		* Flag for knowing if the Mesh has extra trasformation parameters
		*/
		bool extra_transformation;
		/**
		* Representation of our geometry
		*/
		Mesh* mesh;
	public:

		
		/**
		* Load and initialize mesh
		*/
		Object3D(const char* filename);
		virtual ~Object3D(void);
		/**
		* Set parameters for extra tranformation
		* @param angle Angle of Mesh
		* @param pivot Pivot from center of GameObject
		* @param rotvec Vector for applying rotation
		* @see GameObject.h
		*/
		void setExtra(float angle, glm::vec3 pivot, glm::vec3 rotvec);
		/**
		* Draws the shadowmap of Mesh
		* @param program Shader to be applied during the draw call
		* @param angle Angle of the GameObject
		* @param center Center of the GameObject
		* @param rotvec Vector for applying GameObject rotation
		* @param scale Vector for scaling the GameObjcet (together with our Mesh)
		* @see Shader.h
		* @see GameObject.h
		*/
		void DrawShadowmap(Shader& program, float angle, glm::vec3 center, glm::vec3 rotvec, glm::vec3 scale);
		/**
		* Draw of Mesh
		* @param program Shader to be applied during the draw call
		* @param angle Angle of the GameObject
		* @param center Center of the GameObject
		* @param rotvec Vector for applying GameObject rotation
		* @param scale Vector for scaling the GameObjcet (together with our Mesh)
		* @see Shader.h
		* @see GameObject.h
		*/
		void Draw(Shader& program, float angle, glm::vec3 center, glm::vec3 rotvec, glm::vec3 scale);
		/**
		* Update Parameters in order to achieve movement
		* @param time Time the the Game is running for
		*/
		virtual void Update(float time) {}


		glm::vec3 getMinOBB(void);

		glm::vec3 getMaxOBB(void);
	};

}

#endif //!OBJECT_3D
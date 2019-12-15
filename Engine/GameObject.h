#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Mesh.h"
#include "Shader.h"

namespace Engine {

	class ENGINE_API GameObject {
	protected:
		Mesh* mesh;
		glm::mat4 transformation_matrix;
		glm::mat4 normal_matrix;
	public:
		virtual ~GameObject(void) {}
		void Draw(Shader& program);
		void DrawShadowmap(Shader& program);
		virtual void Update(float time) = 0;
	};

	
}

#endif // !GAME_OBJECT_H

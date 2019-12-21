#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Object3D.h"
#include "EventSystem.h"


namespace Engine {
	
	class ENGINE_API GameObject: public ClickListener, KeyListener{
	private:
		bool is_touched;
	protected:
		glm::vec3 center;
		glm::vec3 rotation_vector;
		float angle;
		glm::vec3 scale;
		std::vector<Object3D*> objects;
	public:
		virtual ~GameObject(void);
		void DrawShadowmap(Shader& program);
		void Draw(Shader& program);
		virtual void Update(float time);
		virtual bool onEvent(const SDL_Event& event) override;
		virtual bool onMousePress(const SDL_Event& event) override;
		virtual bool onMouseRelease(const SDL_Event& event) override;
	};


}

#endif // !GAME_OBJECT_H

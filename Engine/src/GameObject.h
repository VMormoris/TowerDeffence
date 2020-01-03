#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Object3D.h"
#include "Events/EventSystem.h"

namespace Engine {
	
	class ENGINE_API GameObject: public ClickListener, KeyListener{
	private:
		bool is_touched;
		bool is_fordelete;
	protected:
		glm::vec3 center;
		glm::vec3 rotation_vector=glm::vec3(0.f,1.f,0.f);
		float angle;
		glm::vec3 scale;
		std::vector<Object3D*> objects;
	public:
		GameObject() { is_fordelete = is_touched = false; angle = glm::radians(0.0f); }
		virtual ~GameObject(void);
		virtual void DrawShadowmap(Shader& program);
		virtual void Draw(Shader& program);
		virtual void Update(float time);
		virtual bool onEvent(const SDL_Event& event) override;
		virtual bool onMousePress(const SDL_Event& event) override;
		virtual bool onMouseRelease(const SDL_Event& event) override;
		void DeleteASAP(void) { is_fordelete = true; }
		bool isForDelete(void) { return is_fordelete; }
	};


}

#endif // !GAME_OBJECT_H

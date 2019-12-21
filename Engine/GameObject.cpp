#include "engpch.h"
#include "GameObject.h"

namespace Engine {

	GameObject::~GameObject(void) {
		for (Object3D* obj : objects)  delete obj; 
		objects.clear();
	}


	void GameObject::DrawShadowmap(Shader& program) {
		for (Object3D* obj : objects)
			obj->DrawShadowmap(program, angle, center, rotation_vector, scale);
	}

	void GameObject::Draw(Shader& program) {
		for (Object3D* obj : objects)
			obj->Draw(program, angle, center, rotation_vector, scale);
	}

	void GameObject::Update(float time) {
		for (Object3D* obj : objects)
			obj->Update(time);
	}


	bool GameObject::onMousePress(const SDL_Event& event) { return false; }
	bool GameObject::onMouseRelease(const SDL_Event& event) { return false; }

	bool GameObject::onEvent(const SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) return onKeyPress(event);
		else if (event.type == SDL_KEYUP) return onKeyRelease(event);
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			is_touched = false;
			float w = MouseListener::SCREEN_WIDTH;
			float h = MouseListener::SCREEN_HEIGHT;

			glm::vec2 current = glm::vec2(event.button.x, h - event.button.y);
			glm::mat4 proj = MouseListener::PROJECTION_MATRIX;
			glm::mat4 view = MouseListener::VIEW_MATRIX;
			
			for (Object3D* obj : objects) {

				glm::vec3 minOBB = center + (glm::rotate(obj->getMinOBB(), angle, rotation_vector) * scale);
				glm::vec3 maxOBB = center + (glm::rotate(obj->getMaxOBB(), angle, rotation_vector) * scale);
				glm::vec3 OBB[8] = {glm::vec3(minOBB.x, minOBB.y, minOBB.z), glm::vec3(minOBB.x, minOBB.y, maxOBB.z), glm::vec3(maxOBB.x , minOBB.y, minOBB.z), glm::vec3(maxOBB.x , minOBB.y, maxOBB.z),
								glm::vec3(minOBB.x, maxOBB.y, minOBB.z), glm::vec3(minOBB.x, maxOBB.y, maxOBB.z), glm::vec3(maxOBB.x , maxOBB.y, minOBB.z), glm::vec3(maxOBB.x , maxOBB.y, maxOBB.z) };
				glm::vec2 screenOBB[8] = { utils::project(OBB[0],proj, view, w, h), utils::project(OBB[1],proj, view, w, h), utils::project(OBB[2],proj, view, w, h), utils::project(OBB[3],proj, view, w, h),
								utils::project(OBB[4],proj, view, w, h), utils::project(OBB[5],proj, view, w, h), utils::project(OBB[6],proj, view, w, h), utils::project(OBB[7],proj, view, w, h) };
				
				float minx = std::min({ screenOBB[0].x, screenOBB[1].x, screenOBB[2].x, screenOBB[3].x, screenOBB[4].x, screenOBB[5].x, screenOBB[6].x, screenOBB[7].x });
				float maxx = std::max({ screenOBB[0].x, screenOBB[1].x, screenOBB[2].x, screenOBB[3].x, screenOBB[4].x, screenOBB[5].x, screenOBB[6].x, screenOBB[7].x });
				float miny = std::min({ screenOBB[0].y, screenOBB[1].y, screenOBB[2].y, screenOBB[3].y, screenOBB[4].y, screenOBB[5].y, screenOBB[6].y, screenOBB[7].y });
				float maxy = std::max({ screenOBB[0].y, screenOBB[1].y, screenOBB[2].y, screenOBB[3].y, screenOBB[4].y, screenOBB[5].y, screenOBB[6].y, screenOBB[7].y });

				if (current.x > minx&& current.x <maxx && current.y>miny&& current.y < maxy) {
					is_touched = true;
					prevCoords = current;
					return onMousePress(event);
				}

			}
			prevCoords = current;
			return false;
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			glm::vec2 current = glm::vec2(event.button.x, MouseListener::SCREEN_HEIGHT - event.button.y);
			glm::vec2 dcoords = glm::vec2(glm::abs(current.x - prevCoords.x), glm::abs(current.y - prevCoords.y));
			
			if (is_touched && (dcoords.x <= CLICK_THRESHOLD && dcoords.y <= CLICK_THRESHOLD)) { is_touched = false; return onClick(event); }
			else if (is_touched) { is_touched = false; return onMouseRelease(event); }
			else return false;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			if (is_touched) return onMouseMove(event);
			else return false;
		}
		else return false;
	}

}

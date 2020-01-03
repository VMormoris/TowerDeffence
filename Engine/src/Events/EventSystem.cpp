#include "../../engpch.h"
#include "EventSystem.h"


namespace Engine {

	
	bool EventDispatcher::DispatchEvent(const SDL_Event& event) {
		for (EventDispatcher* dispatcher : dispachers) {
			if(dispatcher->DispatchEvent(event)) return true;
		}
		for (EventListener* listener : listeners) {
			if(listener->onEvent(event)) return true;
		}
		return onEvent(event);
	}

	bool EventDispatcher::onEvent(const SDL_Event& event) { return false; }

	bool MouseListener::onEvent(const SDL_Event& event) {
		if (event.type == SDL_MOUSEBUTTONDOWN) { 
			if (event.button.button != SDL_BUTTON_MIDDLE) return onMousePress(event);
			else return onWheelPress(event);
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button != SDL_BUTTON_MIDDLE) return onMouseRelease(event);
			else return onWheelRelease(event);
		}
		else if (event.type == SDL_MOUSEMOTION)  return onMouseMove(event);
		else if (event.type == SDL_MOUSEWHEEL) {
			int x = event.wheel.x;
			int y = event.wheel.y;
			if (y > 0) return onWheelUp(event);
			else if (y < 0) return onWheelDown(event);
		}
		return false;
	}

	glm::mat4 MouseListener::PROJECTION_MATRIX;
	glm::mat4 MouseListener::VIEW_MATRIX;
	float MouseListener::SCREEN_WIDTH = 0.f;
	float MouseListener::SCREEN_HEIGHT = 0.f;
	
	bool MouseListener::onMousePress(const SDL_Event& event) { return false; }
	bool MouseListener::onMouseRelease(const SDL_Event& event) { return false; }
	bool MouseListener::onMouseMove(const SDL_Event& event) { return false; }
	bool MouseListener::onWheelPress(const SDL_Event& event) { return false; }
	bool MouseListener::onWheelRelease(const SDL_Event& event) { return false; }
	bool MouseListener::onWheelUp(const SDL_Event& event) { return false; }
	bool MouseListener::onWheelDown(const SDL_Event& event) { return false; }

	bool ClickListener::onEvent(const SDL_Event& event) { 
		return MouseListener::onEvent(event);
	}

	bool ClickListener::onMousePress(const SDL_Event& event) {
		if (event.button.button == SDL_BUTTON_LEFT)
			prevCoords = glm::vec2(event.button.x, event.button.y);
		return false;
	}

	bool ClickListener::onMouseRelease(const SDL_Event& event) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			glm::vec2 dcoords = glm::vec2(glm::abs(event.button.x - prevCoords.x), glm::abs(event.button.y - prevCoords.y));
			if (dcoords.x <= CLICK_THRESHOLD && dcoords.y <= CLICK_THRESHOLD) return onClick(event);
			return false;
		}
	}

	bool ClickListener::onClick(const SDL_Event& event) { return false; }

	bool KeyListener::onEvent(const SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) return onKeyPress(event);
		else if (event.type == SDL_KEYUP) return onKeyRelease(event);
		else return false;
	}

	bool KeyListener::onKeyPress(const SDL_Event& event) { return false; }
	bool KeyListener::onKeyRelease(const SDL_Event& event) { return false; }

	bool WindowListener::onEvent(const SDL_Event& event) {
		if (event.type == SDL_QUIT) return onClose(event);
		else if (event.type == SDL_WINDOWEVENT_RESIZED) return onResize(event);
		else return false;
	}

	bool WindowListener::onClose(const SDL_Event& event) { return false; }
	bool WindowListener::onResize(const SDL_Event& event) { return false; }

}
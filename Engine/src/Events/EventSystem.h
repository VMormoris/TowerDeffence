#pragma once
#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H
#include "../../engpch.h"
#include "../Core/Core.h"

#define CLICK_THRESHOLD 4//Threshold for Mouse Click (Was 4 in my laptop's Registry)

namespace Engine{

	
	class ENGINE_API EventListener {
	public:
		virtual bool onEvent(const SDL_Event& event) { return false; };
	};

	class ENGINE_API EventDispatcher : virtual public EventListener {
	protected:
		std::vector<EventDispatcher*> dispachers;
		std::vector<EventListener*> listeners;
	public:
		bool DispatchEvent(const SDL_Event& event);
		virtual bool onEvent(const SDL_Event& event) override;
	};

	class ENGINE_API MouseListener : virtual public EventListener {
	protected:
		static glm::mat4 PROJECTION_MATRIX;
		static glm::mat4 VIEW_MATRIX;
		static float SCREEN_WIDTH;
		static float SCREEN_HEIGHT;
	public:
		virtual bool onEvent(const SDL_Event& event) override;
		virtual bool onMousePress(const SDL_Event& event);
		virtual bool onMouseMove(const SDL_Event& event);
		virtual bool onMouseRelease(const SDL_Event& event);
		virtual bool onWheelPress(const SDL_Event& event);
		virtual bool onWheelRelease(const SDL_Event& event);
		virtual bool onWheelUp(const SDL_Event& event);
		virtual bool onWheelDown(const SDL_Event& event);
	};

	class ENGINE_API ClickListener : public MouseListener {
	protected:
		glm::vec2 prevCoords;
	public:
		virtual bool onEvent(const SDL_Event& event) override;
		virtual bool onMousePress(const SDL_Event& event) override;
		virtual bool onMouseRelease(const SDL_Event& event) override;
		virtual bool onClick(const SDL_Event& event);
	};

	class ENGINE_API KeyListener : virtual public EventListener {
	public:
		virtual bool onEvent(const SDL_Event& event) override;
		virtual bool onKeyPress(const SDL_Event& event);
		virtual bool onKeyRelease(const SDL_Event& event);
	};

	class ENGINE_API WindowListener : virtual public EventListener {
	public:
		virtual bool onEvent(const SDL_Event& event) override;
		virtual bool onResize(const SDL_Event& event);
		virtual bool onClose(const SDL_Event& event);
	};

}
#endif//!EVENT_SYSTEM_H
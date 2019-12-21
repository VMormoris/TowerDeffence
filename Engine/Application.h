#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "utils.h"
#include "Scene.h"

namespace Engine {

	/**
	* This class must be inhereted by a program
	*	Contains the root of our program
	*/
	class ENGINE_API Application : public EventDispatcher, MouseListener, KeyListener, WindowListener {
	protected:
		/**
		* Title of the Window that this Application will create
		*/
		std::string WINDOW_TITLE;
		/**
		* Window width
		*/
		int width;
		/**
		* Window height
		*/
		int height;
		/**
		*All scenes that maybe be renderer at some point(aka the different levelsand Menus)
		* @see Scene
		*/
		std::vector<Scene*> scenes;
		/**
		* Index of the currently active scene
		*/
		int active_scene;
		/**
		* Camera that will be used by the Scenes
		* @see Camera
		*/
		Camera camera;
		/**
		* Flag While true the application is Runing when false the application stops
		*/
		bool running;
	public:
		/**
		* Default Constructor used for initialization of the fields
		*	TOBE: overrided
		*/
		Application(void);
		/**
		* Free occupied memory
		*	TOBE: overrided
		*/
		virtual ~Application(void) {}
		/**
		* Function for the hall life cycle of our application
		*	- Window creation
		*   - Render of currently Active Scene
		*/
		void Run(void);
		/**
		* Handle every event that hasn't be handled already
		*	TOBE: overrided WE HAVE MULTIPLE LISTENERS
		*/
		bool onEvent(const SDL_Event& event) override;
		/**
		* This function will be called when there is an event for closing the Window of our application
		*/
		bool onClose(const SDL_Event& event) override;
		/**
		* This function will be called when the Window of our application is Resized
		*/
		bool onResize(const SDL_Event& event) override;
	};
}
#endif//!APPLICATION_H

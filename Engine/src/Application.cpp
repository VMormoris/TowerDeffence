#include "../engpch.h"
#include "Application.h"
#include "AssetManagment/TextureManager.h"
#include "AssetManagment/MeshManager.h"
#include "AssetManagment/AudioManager.h"

namespace Engine {

	Application::Application(void) {
		active_scene = -1;
		WINDOW_TITLE = "Engine Test";
		running = true;
		camera = Camera(glm::vec3(1, 4, -10), glm::vec3(0, 0, 0));
		MouseListener::PROJECTION_MATRIX = camera.getProjectionMatrix();
		MouseListener::VIEW_MATRIX = camera.getViewMatrix();
		width = 0; 
		height = 0;
		MouseListener::SCREEN_WIDTH = width;
		MouseListener::SCREEN_HEIGHT = height;
		time = 0.f;
	}

	void Application::Run(){
		if (active_scene == -1) {
			printf("%s\n", "No scene to rederer... Exiting!");
			return;
		}

		SDL_GLContext gContext;
		SDL_Window* window = Engine::utils::CreateFullscreenWindow(WINDOW_TITLE.c_str(), gContext);
		AudioManager::Init();
		Music* instrumental=AudioManager::RequestMusic("../Assets/Music/Eclosion/Salmo_-_01_-_Eclosion.mp3");
		instrumental->Play();
		if (window == NULL) {
			printf("%s\n", "Creation of Window Failed!");
			system("pause");
			exit(EXIT_FAILURE);
		}

		Engine::utils::GPU_INFO();

		SDL_GetWindowSize(window, &width, &height);

		MouseListener::SCREEN_WIDTH = width;
		MouseListener::SCREEN_HEIGHT = height;
		bool engine_initialized = scenes[active_scene]->Init(width, height);
		if (engine_initialized) printf("Renderer initialization complete.\n");
		else {
			printf("Renderer initialization failed!\n");
			system("pause");
			exit(EXIT_FAILURE);
		}
		auto simulation_start = std::chrono::steady_clock::now();
		SDL_Event event;
		
		while (running) {
			while (SDL_PollEvent(&event)) { DispatchEvent(event); }
			// Compute the ellapsed time
			auto simulation_end = std::chrono::steady_clock::now();
			float dt = std::chrono::duration <float>(simulation_end - simulation_start).count();//in seconds
			simulation_start = std::chrono::steady_clock::now();

			Loop(dt);

			//Update screen (swap buffer for double buffering)
			SDL_GL_SwapWindow(window);
		}

		delete scenes[active_scene];
		scenes[active_scene] = NULL;

		TextureManager::Clear();
		MeshManager::Clear();
		AudioManager::Clear();

		SDL_GL_DeleteContext(gContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	bool Application::onEvent(const SDL_Event& event) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT:
			return WindowListener::onEvent(event);
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			return KeyListener::onEvent(event);
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			return MouseListener::onEvent(event);
		default:
			return true;
		}
	}

	bool Application::onResize(const SDL_Event& event) {
		width = event.window.data1;
		height = event.window.data2;
		(scenes[active_scene])->ResizeBuffers(width, height);
		MouseListener::SCREEN_WIDTH = width;
		MouseListener::SCREEN_HEIGHT = height;
		return true;
	}

	bool Application::onClose(const SDL_Event& event) {
		running = false;
		return true;
	}

}
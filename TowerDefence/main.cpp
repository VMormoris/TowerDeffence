#include "MainScene.h"
#include <chrono>
#include <iostream>

#define WINDOW_NAME "Test Engine"
#define WIDTH 1600
#define HEIGHT 900

int main(int argc, char* argv[]){
	

	//Initialize window
	SDL_GLContext gContext;
	SDL_Window* window = Engine::utils::CreateFullscreenWindow(WINDOW_NAME, gContext);
	if (window == NULL) {
		std::cout << "Creation of Window failed!" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}
	Engine::utils::GPU_INFO();


	Engine::Camera* camera = new Engine::Camera();

	Engine::Scene* scene = new MainScene(camera);

	bool is_mouse_pressed = false;
	glm::vec2 mouseCoords(0);
	
	bool engine_initialized = scene->Init(WIDTH, HEIGHT);
	if(engine_initialized)
		std::cout << "Renderer initialization complete." << std::endl;
	else {
		std::cout << "Renderer initialization failed!" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}
	auto simulation_start = std::chrono::steady_clock::now();


	while (true) {

		//Poll Events
		SDL_Event event;//Variable that holds the event being polled
		bool flag = false;//Exit flag
		while (SDL_PollEvent(&event)) {//Loop for existing events waiting to be handled

			if (event.type == SDL_QUIT) { flag = true; }//Close window
			else if (event.type == SDL_WINDOWEVENT) {//Other window events
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) { //Resize window
					scene->ResizeBuffers(event.window.data1, event.window.data2);
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_w) camera->MoveUpwards(true);
				else if (event.key.keysym.sym == SDLK_s) camera->MoveDownwards(true);
				else if (event.key.keysym.sym == SDLK_d) camera->MoveRight(true);
				else if (event.key.keysym.sym == SDLK_a) camera->MoveLeft(true);
				else if (event.key.keysym.sym == SDLK_SPACE) scene->ChangePauseState();
				else if (event.key.keysym.sym == SDLK_ESCAPE) flag = true;
			}
			else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_w) camera->MoveUpwards(false);
				else if (event.key.keysym.sym == SDLK_s) camera->MoveDownwards(false);
				else if (event.key.keysym.sym == SDLK_d) camera->MoveRight(false);
				else if (event.key.keysym.sym == SDLK_a) camera->MoveLeft(false);
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x = event.button.x;
					int y = event.button.y;
					is_mouse_pressed = (event.type == SDL_MOUSEBUTTONDOWN);
					mouseCoords = glm::vec2(x, y);
				}
			}
			else if (event.type == SDL_MOUSEMOTION) {
				int x = event.motion.x;
				int y = event.motion.y;
				if (is_mouse_pressed) {
					camera->LookAt(glm::vec2(x, y) - mouseCoords);
					mouseCoords = glm::vec2(x, y);
				}
			}
			else if (event.type == SDL_MOUSEWHEEL) {
				int x = event.wheel.x;
				int y = event.wheel.y;
				if (y > 0) camera->MoveForward(true);
				else if (y < 0) camera->MoveBackwards(true);
			}

		}

		// Compute the ellapsed time
		auto simulation_end = std::chrono::steady_clock::now();
		float dt = std::chrono::duration <float>(simulation_end - simulation_start).count(); // in seconds
		simulation_start = std::chrono::steady_clock::now();

		// Update
		scene->Update(dt);
		camera->MoveBackwards(false);
		camera->MoveForward(false);

		// Draw
		scene->Render();

		//Update screen (swap buffer for double buffering)
		SDL_GL_SwapWindow(window);

		if (flag) break;

	}

	delete scene;
	delete camera;

	Engine::TextureManager::Clear();

	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	std::cout << "Closing..." << std::endl;
	
	exit(0);
}

#include "Game.h"
#include <chrono>
#include <iostream>

#define WINDOW_NAME "Test Engine"
#define WIDTH 1600
#define HEIGHT 900

int main(int argc, char* argv[]){
	

	//Initialize window
	SDL_GLContext gContext;
	SDL_Window* window = Engine::utils::CreateWindow(WINDOW_NAME, WIDTH, HEIGHT, gContext);
	if (window == NULL) {
		std::cout << "Creation of Window failed!" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}
	
	Engine::Renderer* renderer = new Game();
	bool engine_initialized = renderer->Init(WIDTH, HEIGHT);// , & setupBuffers, & setupProgram);//Engine Initialization
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

			if (event.type == SDL_QUIT) { flag = true; break; }//Close window
			else if (event.type == SDL_WINDOWEVENT) {//Other window events
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) { //Resize window
					renderer->ResizeBuffers(event.window.data1, event.window.data2);
				}
			}

		}

		// Compute the ellapsed time
		auto simulation_end = std::chrono::steady_clock::now();
		float dt = std::chrono::duration <float>(simulation_end - simulation_start).count(); // in seconds
		simulation_start = std::chrono::steady_clock::now();

		// Update
		renderer->Update(dt);

		// Draw
		renderer->Render();

		//Update screen (swap buffer for double buffering)
		SDL_GL_SwapWindow(window);

		if (flag) break;

	}

	delete renderer;

	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	std::cout << "Closing..." << std::endl;
	exit(0);
}

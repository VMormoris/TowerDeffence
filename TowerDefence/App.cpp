#include "App.h"

App::App(void) : Application() {
	Engine::Scene* scene = new MainScene(&camera);
	scenes.push_back(scene);
	active_scene = 0;
	dispachers.push_back(scene);
	is_rightClick = false;
}

bool App::onKeyPress(const SDL_Event& event) {
	switch (event.key.keysym.sym) {
	case SDLK_ESCAPE:
		running = false;
		return true;
	case SDLK_w:
		camera.MoveUpwards(true);
		return true;
	case SDLK_s:
		camera.MoveDownwards(true);
		return true;
	case SDLK_d:
		camera.MoveRight(true);
		return true;
	case SDLK_a:
		camera.MoveLeft(true);
		return true;
	default:
		return false;
	}
}

bool App::onKeyRelease(const SDL_Event& event) {
	switch (event.key.keysym.sym) {
	case SDLK_w:
		camera.MoveUpwards(false);
		return true;
	case SDLK_s:
		camera.MoveDownwards(false);
		return true;
	case SDLK_d:
		camera.MoveRight(false);
		return true;
	case SDLK_a:
		camera.MoveLeft(false);
		return true;
	default:
		return false;
	}
}

bool App::onMousePress(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_RIGHT) {
		is_rightClick = true;
		prevCoords = glm::vec2(event.button.x, event.button.y);
		return true;
	}
	else return false;
}

bool App::onMouseMove(const SDL_Event& event) {
	if (is_rightClick) {
		glm::vec2 currentCoords = glm::vec2(event.button.x, event.button.y);
		camera.LookAt(currentCoords - prevCoords);
		prevCoords = currentCoords;
		return true;
	}
	else return false;
}

bool App::onMouseRelease(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_RIGHT) {
		is_rightClick = false;
		return true;
	}
	return false;
}
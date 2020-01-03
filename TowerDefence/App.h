#pragma once
#ifndef APP_H
#define APP_H

#include "MainScene.h"

class App : public Engine::Application {
private:
	bool is_rightClick;
	glm::vec2 prevCoords;
public:
	App(void);
	void Loop(float dt) override;
	bool onKeyPress(const SDL_Event& event) override;
	bool onKeyRelease(const SDL_Event& event) override;
	bool onMousePress(const SDL_Event& event) override;
	bool onMouseRelease(const SDL_Event& event) override;
	bool onMouseMove(const SDL_Event& event) override;
	bool onWheelUp(const SDL_Event& event) override;
	bool onWheelDown(const SDL_Event& event) override;
};

#endif // !APP_H

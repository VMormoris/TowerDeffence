#pragma once
#ifndef _GAME_H
#define _GAME_H
#include "Knossos.h"
#include "Skeleton.h"
#include "Pirate.h"


class MainScene : public Engine::Scene {
private:


	std::vector<Engine::GameObject*> objects;
	Engine::SpotLight lamp;


	bool InitRenderingTechniques(void) override;
	bool InitGeometricMeshes(void) override;
	bool InitLightSources(void) override;

public:

	MainScene(Engine::Camera* cam);
	~MainScene(void);

	void Update(float dt);

	void RenderShadowmap(void) override;
	void RenderGeometry(void) override;
	void RenderOutFB(void) override;

};


#endif // !_GAME_H

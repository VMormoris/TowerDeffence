#pragma once
#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H
#include <Engine.h>
#include "Pirate.h"
#include "Field.h"

class MainScene : public Engine::Scene{
public:
	MainScene(Engine::Camera* camera);
	~MainScene();

	void Update(float dt) override;

	void RenderShadowmap(void) override;
	void RenderGeometry(void) override;
	void RenderOutFB(void) override;

private:
	Engine::SpotLight lamp;
	bool InitRenderingTechniques(void) override;
	bool InitGeometricMeshes(void) override;
	bool InitLightSources(void) override;
};


#endif // !MAIN_SCENE_H

#pragma once
#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H
#include <Engine.h>
#include "Tower.h"
#include "Pirate.h"

class MainScene : public Engine::Scene{
public:
	MainScene(Engine::Camera* camera);
	~MainScene();

	void Update(float dt) override;
	void DetectCollisions(void) override;
	void DeleteNotUsedObjects(void) override;

	void RenderShadowmap(void) override;
	void RenderGeometry(void) override;
	void RenderOutFB(void) override;

private:
	Engine::SpotLight lamp;
	std::vector<CollisionListener*> colliders;
	std::vector<Pirate*> pirates;
	std::vector<Tower*> towers;
	bool InitRenderingTechniques(void) override;
	bool InitGeometricMeshes(void) override;
	bool InitLightSources(void) override;
};


#endif //!MAIN_SCENE_H
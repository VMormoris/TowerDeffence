#pragma once
#ifndef _GAME_H
#define _GAME_H
#include "Knossos.h"
#include "Skeleton.h"
#include "Pirate.h"


class Game : public Engine::Renderer {
private:

	Knossos* knossos;
	Skeleton* skeleton;
	Pirate* pirate;

	bool InitRenderingTechniques(void) override;
	bool InitIntermediateShaderBuffers(void) override;
	bool InitCommonItems(void) override;
	bool InitGeometricMeshes(void) override;


public:

	Game(void);
	~Game(void);

	void Update(float dt);

	void RenderGeometry(void) override;


};


#endif // !_GAME_H

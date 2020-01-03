#pragma once
#ifndef ROAD_H
#define ROAD_H
#include <Engine.h>

class Tile: public Engine::Object3D{
public:
	Tile(void) :Object3D("../Assets/Terrain/road.obj") {}
};

class Road : public Engine::GameObject {
public:
	
	Road(void);
	static float roadmap[29][3];

};


#endif // !ROAD_H

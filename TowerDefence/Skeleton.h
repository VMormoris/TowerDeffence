#pragma once
#ifndef SKELETON_H
#define SKELETON_H

#include <Engine.h>

class Skeleton : public Engine::GameObject {
public:
	Skeleton(void);
	~Skeleton(void);
	void Update(float time);
};

#endif//!SPIRATE_H
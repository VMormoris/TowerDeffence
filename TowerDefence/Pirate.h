#pragma once
#ifndef PIRATE_H
#define PIRATE_H
#include <Engine.h>
class Pirate : public Engine::GameObject {
public:
	Pirate(void);
	~Pirate(void) override;
	void Update(float time) {}
};

#endif // !KNOSSOS_H
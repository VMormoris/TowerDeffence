#pragma once
#ifndef KNOSSOS_H
#define KNOSSOS_H
#include <Engine.h>
class Knossos: public Engine::GameObject {
public:
	Knossos(void);
	~Knossos(void) override;
	void Update(float time) {}
};

#endif // !KNOSSOS_H

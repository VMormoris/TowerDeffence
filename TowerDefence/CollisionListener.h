#pragma once
#ifndef COLLISION_LISTENER_H
#define COLLISION_LISTENER_H
#include <string>

class CollisionListener {
protected:
	std::string tag;
public:
	std::string GetTag(void) { return tag; }
	virtual bool onCollision(CollisionListener* other) = 0;
	
};


#endif // !COLLISION_LISTENER_H

#pragma once
#include <cyclone\core.h>
#include <cyclone\collide_fine.h>

class Box : public cyclone::CollisionBox
{
private:
	cyclone::RigidBody *body;
public:
	Box(void);
	virtual ~Box(void);
};


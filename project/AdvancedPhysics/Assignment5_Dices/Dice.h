#pragma once

#include <cyclone\collide_fine.h>

class Dice
{
public:
	Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z);
	~Dice(void);
	void integrate(cyclone::real duration);
	void render();
private:
	cyclone::CollisionSphere* vertices[8];
};


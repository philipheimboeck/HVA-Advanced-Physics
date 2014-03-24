#pragma once

#include <cyclone\collide_fine.h>


class Dice
{
public:
	Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z);
	~Dice(void);
	void integrate(cyclone::real duration);
	void render();
	void createContactsDice(Dice *dice, cyclone::CollisionData *data);
	void createContactsPlane(cyclone::CollisionPlane *plane, cyclone::CollisionData *data);
	void recalculate();
private:

	cyclone::CollisionBox box;
	cyclone::CollisionSphere sphere;
};


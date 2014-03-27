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

	void setOrientation(cyclone::real r, cyclone::real i, cyclone::real j, cyclone::real k);
	void setOrientation(cyclone::Quaternion &orientation);
	void setRotation(cyclone::real x, cyclone::real y, cyclone::real z);
	void setRotation(cyclone::Vector3 &rotation);
	void setAcceleration(cyclone::real x, cyclone::real y, cyclone::real z);
	void setAcceleration(cyclone::Vector3 &rotation);

	bool verbose;
private:

	cyclone::CollisionBox box;
	cyclone::CollisionSphere sphere;
};


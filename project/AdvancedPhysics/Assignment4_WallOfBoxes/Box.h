#pragma once
#include <cyclone\core.h>
#include <cyclone\collide_fine.h>


class Box : public cyclone::CollisionBox
{
private:
	void init(cyclone::Vector3 halfSize);
	cyclone::real massFactor;

public:
	Box(void);
	Box(cyclone::Vector3 halfSize);
	

	virtual ~Box(void);

	void render();

	void setMass(const cyclone::real massFactor);
	cyclone::real getMass();
	void setPosition(const cyclone::real x, const cyclone::real y, const cyclone::real z);
	void setPosition(const cyclone::Vector3 &position);
	void setRotation(const cyclone::real x, const cyclone::real y, const cyclone::real z);
	void setRotation(const cyclone::Vector3 &rotation);
	void setDamping(const cyclone::real linearDamping, const cyclone::real angularDamping);
	void setOrientation(const cyclone::real r, const cyclone::real i, const cyclone::real j, const cyclone::real k);
	void setOrientation(const cyclone::Quaternion &orientation);

	void calculateInertia();
	void recalculate();

	void integrate(cyclone::real duration);
};


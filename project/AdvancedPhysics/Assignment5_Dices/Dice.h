/**
 * Dice
 * Author: Philip Heimböck and Florian Mathis
 * 
 * Represents a dice with different colored sides and rounded edges
 * A dice consists of a CollisionBox and a CollisionSphere. A dice only collides with it's environment
 * if both, the box and sphere, are colliding.
 */

#pragma once

#include <cyclone\collide_fine.h>

class Dice
{
public:
	/**
	 * Constructor
	 * Creates a new dice with the size of 2x halfsize, and the the position of X, Y and Z
	 */
	Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z);
	~Dice(void);

	/**
	 * Update the position of the dice
	 */
	void integrate(cyclone::real duration);

	/**
	 * Renders the colored dice with rounded edges
	 */
	void render();

	/**
	 * Check for contacts with another dice
	 * The contact only exists, if spheres and the boxes of both dices are colliding with each other
	 */
	void createContactsDice(Dice *dice, cyclone::CollisionData *data);
	
	/**
	 * Check for contacts with an plane
	 * The contact only exists, if both, the sphere and the box, collides with the plane
	 */
	void createContactsPlane(cyclone::CollisionPlane *plane, cyclone::CollisionData *data);

	/**
	 * Recalculate the inner data
	 */
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


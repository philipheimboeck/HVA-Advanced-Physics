#include "Dice.h"
#include <gl/glut.h>
#include "squadric.h"

Dice::Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.halfSize = cyclone::Vector3(halfsize, halfsize, halfsize);
	box.body = new cyclone::RigidBody();

	cyclone::real mass = box.halfSize.x * box.halfSize.y * box.halfSize.z * 2.0f;
	box.body->setMass(mass);

	cyclone::Matrix3 tensor;
	tensor.setBlockInertiaTensor( box.halfSize, mass );
    box.body->setInertiaTensor( tensor );

	box.body->setDamping(1.0, 1.0);
	box.body->clearAccumulators();
	box.body->setAcceleration(cyclone::Vector3::GRAVITY);

	box.body->setCanSleep(true);
	box.body->setAwake(true);
	
		
	sphere.body = new cyclone::RigidBody();
	sphere.radius = halfsize * 0.75;
	
	box.body->setPosition(x, y, z);
	sphere.body->setPosition(x, y, z);

	box.body->calculateDerivedData();

	// Don't display spheres
	verbose = false;
}

Dice::~Dice(void)
{
}

void Dice::recalculate() {
	// Calculate the Transform Matrix
	box.body->calculateDerivedData();

	// Calculate internal contact data
	box.calculateInternals();
}

void Dice::render()
{
	// Get the OpenGL transformation
    GLfloat mat[16];
    box.body->getGLTransform( mat );

    glPushMatrix();
        glMultMatrixf( mat );

        glPushMatrix();
            glScalef( box.halfSize.x, box.halfSize.y, box.halfSize.z );
			sqSolidRoundCube( box.halfSize.x*2, 30, 30 );

			if ( verbose ) glutWireSphere(sphere.radius, 30, 20);
        glPopMatrix();
    glPopMatrix();
}

void Dice::integrate(cyclone::real duration)
{
	box.body->integrate(duration);
	box.calculateInternals();

	// Update the sphere position
    sphere.body->setPosition( box.body->getPosition() );
	sphere.body->setOrientation(box.body->getOrientation() );
}

void Dice::createContactsDice(Dice *dice, cyclone::CollisionData *data)
{
	// If the boxes and spheres of both dices are colliding, the dices are colliding
	if( cyclone::IntersectionTests::boxAndBox( this->box, dice->box ) &&
		cyclone::IntersectionTests::sphereAndSphere( this->sphere, dice->sphere ) )
    {
		cyclone::CollisionDetector::boxAndBox( box, dice->box, data );
    }
}

void Dice::createContactsPlane(cyclone::CollisionPlane *plane, cyclone::CollisionData *data) 
{
	// If box and sphere collide with the plane, then the dice collides
	if( cyclone::IntersectionTests::boxAndHalfSpace( box, *plane ) &&
		cyclone::IntersectionTests::sphereAndHalfSpace( sphere, *plane ) )
    {
        cyclone::CollisionDetector::boxAndHalfSpace( box, *plane, data );
    }
}

void Dice::setOrientation(cyclone::real r,cyclone::real i, cyclone::real j, cyclone::real k)
{
	box.body->setOrientation(r, i, j, k);
}

void Dice::setOrientation(cyclone::Quaternion &orientation)
{
	box.body->setOrientation(orientation);
}

void Dice::setRotation(cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.body->setRotation(x, y, z);
}

void Dice::setRotation(cyclone::Vector3 &rotation)
{
	box.body->setRotation(rotation);
}

void Dice::setAcceleration(cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.body->setAcceleration(x, y, z);
}

void Dice::setAcceleration(cyclone::Vector3 &rotation)
{
	box.body->setAcceleration(rotation);
}
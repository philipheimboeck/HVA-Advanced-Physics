#include "Dice.h"
#include <gl/glut.h>
#include "squadric.h"

Dice::Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.halfSize = cyclone::Vector3(halfsize, halfsize, halfsize);
	box.body = new cyclone::RigidBody();
	box.body->setMass(0.5f);
	box.body->setDamping(1.0, 1.0);
	box.body->clearAccumulators();
	box.body->setAcceleration(cyclone::Vector3::GRAVITY);
	box.body->setCanSleep(true);
	box.body->setAwake(true);
	box.body->calculateDerivedData();
		
	sphere.body = new cyclone::RigidBody();
	sphere.radius = halfsize * 0.75;
	
	box.body->setPosition(x, y, z);
	sphere.body->setPosition(x, y, z);
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
	glColor3f(0, 0, 1);
    GLfloat mat[16];
    box.body->getGLTransform( mat );

    glPushMatrix();
        glMultMatrixf( mat );

        glPushMatrix();
            glScalef( box.halfSize.x, box.halfSize.y, box.halfSize.z );
            sqSolidRoundCube( sphere.radius, 30, 30 );

			glutWireSphere(sphere.radius, 30, 20);
        glPopMatrix();
    glPopMatrix();
}

void Dice::integrate(cyclone::real duration)
{
	box.body->integrate(duration);
	box.calculateInternals();

	// Update the sphere position
    sphere.body->setPosition( box.body->getPosition() );
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
	// If box and sphere are colliding, then the dice collides
	if( cyclone::IntersectionTests::boxAndHalfSpace( box, *plane ) && cyclone::IntersectionTests::sphereAndHalfSpace( sphere, *plane ) )
    {
        cyclone::CollisionDetector::boxAndHalfSpace( box, *plane, data );
    }
}
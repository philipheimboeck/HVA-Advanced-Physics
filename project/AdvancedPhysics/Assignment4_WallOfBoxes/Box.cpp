#include "Box.h"
#include <gl/glut.h>
#include <string>


Box::Box(void)
{
	init(cyclone::Vector3(1, 1, 1));
}

Box::Box(cyclone::Vector3 halfSize)
{
	init(halfSize);
}

void Box::init(cyclone::Vector3 halfSize)
{
	body = new cyclone::RigidBody;
	body->setAcceleration(cyclone::Vector3::GRAVITY);
	this->halfSize = halfSize;
	this->color = cyclone::Vector3(1, 1, 1);
}


Box::~Box(void)
{
	delete body;
}

void Box::render(void) 
{
	// Get the OpenGL transformation
	glColor3f(color.x, color.y, color.z);
    GLfloat mat[16];
    body->getGLTransform(mat);

    glPushMatrix();
    glMultMatrixf(mat);
    glScalef(halfSize.x*2, halfSize.y*2, halfSize.z*2);
    glutSolidCube(1.0f);

	// Write mass
	glColor3f(1, 1, 1);
	glRasterPos3f(0, 0, -.9);
	std::string text = std::to_string((long long)body->getMass()); 
	for ( size_t i = 0; i < text.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text.at(i));
	}

    glPopMatrix();

	
}

void Box::setMass(const cyclone::real massFactor)
{
	this->massFactor = massFactor;
	body->setMass(halfSize.x * halfSize.y * halfSize.z * massFactor);
}

cyclone::real Box::getMass()
{
	return massFactor;
}

void Box::setPosition(const cyclone::real x, const cyclone::real y, const cyclone::real z)
{
	body->setPosition(x, y, z);
}

void Box::setPosition(const cyclone::Vector3 &position)
{
	body->setPosition(position);
}

void Box::setRotation(const cyclone::real x, const cyclone::real y, const cyclone::real z)
{
	body->setRotation(x, y, z);

	body->calculateDerivedData();
	calculateInternals();
}

void Box::setRotation(const cyclone::Vector3 &rotation)
{
	body->setRotation(rotation);

	body->calculateDerivedData();
	calculateInternals();
}

void Box::setDamping(const cyclone::real linearDamping, const cyclone::real angularDamping)
{
	body->setDamping(linearDamping, angularDamping);
}

void Box::setOrientation(const cyclone::real r, const cyclone::real i, const cyclone::real j, const cyclone::real k)
{
	body->setOrientation(r, i, j, k);
}

void Box::setOrientation(const cyclone::Quaternion &orientation)
{
	body->setOrientation(orientation);
}

void Box::calculateInertia()
{
	cyclone::Matrix3 tensor;
	tensor.setBlockInertiaTensor(halfSize, body->getMass());
    body->setInertiaTensor(tensor);
}

void Box::recalculate()
{
	// Calculate the Transform Matrix
	body->calculateDerivedData();

	// Calculate internal contact data
	calculateInternals();
}

void Box::integrate(cyclone::real duration)
{
	body->integrate(duration);
	calculateInternals();
}

void Box::setColor(cyclone::Vector3 color)
{
	this->color = color;
}
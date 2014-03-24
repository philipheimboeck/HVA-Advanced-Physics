#include "Dice.h"
#include <gl/glut.h>

Dice::Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z)
{
	for ( int i = 0; i < 8; i++ )
	{
		vertices[i] = new cyclone::CollisionSphere();
		vertices[i]->body = new cyclone::RigidBody();
		vertices[i]->body->setMass(50.f);
		vertices[i]->body->setDamping(1.0, 1.0);
		vertices[i]->body->clearAccumulators();
		vertices[i]->body->setAcceleration(0,0,0);
		vertices[i]->body->setCanSleep(true);
		vertices[i]->body->setAwake(true);
		vertices[i]->body->calculateDerivedData();
		vertices[i]->radius = halfsize/8;
	}
	
	vertices[0]->body->setPosition(x - halfsize, y - halfsize, z - halfsize);
	vertices[1]->body->setPosition(x + halfsize, y - halfsize, z - halfsize);
	vertices[2]->body->setPosition(x + halfsize, y + halfsize, z - halfsize);
	vertices[3]->body->setPosition(x - halfsize, y + halfsize, z - halfsize);
	vertices[4]->body->setPosition(x - halfsize, y - halfsize, z + halfsize);
	vertices[5]->body->setPosition(x + halfsize, y - halfsize, z + halfsize);
	vertices[6]->body->setPosition(x + halfsize, y + halfsize, z + halfsize);
	vertices[7]->body->setPosition(x - halfsize, y + halfsize, z + halfsize);
}


Dice::~Dice(void)
{
	delete[] vertices;
}

void Dice::render()
{

	// Get the OpenGL transformation
	glColor3f(0, 0, 1);
    GLfloat mat[16];
    
	for ( int i = 0; i < 8; i++ )
	{
		vertices[i]->body->getGLTransform(mat);
		glPushMatrix();
		glMultMatrixf(mat);
		//glTranslatef (0, 0, 0);
		glScalef(vertices[i]->radius*2, vertices[i]->radius*2, vertices[i]->radius*2);
		glutWireSphere(vertices[i]->radius, 20, 20);
		glPopMatrix();
	}
}

void Dice::integrate(cyclone::real duration)
{
	for ( int i = 0; i < 8; i++ )
	{
		vertices[i]->body->integrate(duration);
		vertices[i]->calculateInternals();
	}
}
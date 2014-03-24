#include "Dice.h"
#include <gl/glut.h>

Dice::Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z)
{
	for ( int i = 0; i < 8; i++ )
	{
		vertices[i] = new cyclone::CollisionSphere();
		vertices[i]->body = new cyclone::RigidBody();
		vertices[i]->body->setMass(0.5f);
		vertices[i]->body->setDamping(1.0, 1.0);
		vertices[i]->body->clearAccumulators();
		vertices[i]->body->setAcceleration(0, 0, 0);
		vertices[i]->body->setCanSleep(true);
		vertices[i]->body->setAwake(true);
		vertices[i]->body->calculateDerivedData();
		vertices[i]->radius = halfsize/4;
	}
	
	vertices[0]->body->setPosition(x - halfsize, y - halfsize, z - halfsize);
	vertices[1]->body->setPosition(x + halfsize, y - halfsize, z - halfsize);
	vertices[2]->body->setPosition(x + halfsize, y + halfsize, z - halfsize);
	vertices[3]->body->setPosition(x - halfsize, y + halfsize, z - halfsize);
	vertices[4]->body->setPosition(x - halfsize, y - halfsize, z + halfsize);
	vertices[5]->body->setPosition(x + halfsize, y - halfsize, z + halfsize);
	vertices[6]->body->setPosition(x + halfsize, y + halfsize, z + halfsize);
	vertices[7]->body->setPosition(x - halfsize, y + halfsize, z + halfsize);

	// Join the vertices
	joints[0].set(vertices[0]->body, cyclone::Vector3(-halfsize, 0, 0), 
		vertices[1]->body, cyclone::Vector3(halfsize, 0, 0), 1);
	joints[1].set(vertices[1]->body, cyclone::Vector3(0, -halfsize, 0), 
		vertices[2]->body, cyclone::Vector3(0, halfsize, 0), 1);
	joints[2].set(vertices[2]->body, cyclone::Vector3(halfsize, 0, 0), 
		vertices[3]->body, cyclone::Vector3(-halfsize, 0, 0), 1);
	joints[3].set(vertices[3]->body, cyclone::Vector3(0, halfsize, 0), 
		vertices[0]->body, cyclone::Vector3(0, -halfsize, 0), 1);

	joints[4].set(vertices[4]->body, cyclone::Vector3(-halfsize, 0, 0), 
		vertices[5]->body, cyclone::Vector3(halfsize, 0, 0), 1);
	joints[5].set(vertices[5]->body, cyclone::Vector3(0, -halfsize, 0), 
		vertices[6]->body, cyclone::Vector3(0, halfsize, 0), 1);
	joints[6].set(vertices[6]->body, cyclone::Vector3(halfsize, 0, 0), 
		vertices[7]->body, cyclone::Vector3(-halfsize, 0, 0), 1);
	joints[7].set(vertices[7]->body, cyclone::Vector3(0, halfsize, 0), 
		vertices[4]->body, cyclone::Vector3(0, -halfsize, 0), 1);

	joints[8].set(vertices[0]->body, cyclone::Vector3(0, 0, -halfsize), 
		vertices[4]->body, cyclone::Vector3(0, 0, halfsize), 1);
	joints[9].set(vertices[1]->body, cyclone::Vector3(0, 0, 0), 
		vertices[5]->body, cyclone::Vector3(0, 0, halfsize), 1);
	joints[10].set(vertices[2]->body, cyclone::Vector3(0, 0, 0), 
		vertices[6]->body, cyclone::Vector3(0, 0, halfsize), 1);
	joints[11].set(vertices[3]->body, cyclone::Vector3(0, 0, 0), 
		vertices[7]->body, cyclone::Vector3(0, 0, halfsize), 1);
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
#include "Dice.h"
#include <gl/glut.h>

Dice::Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z)
{
	for ( int i = 0; i < 8; i++ )
	{
		vertices[i] = new cyclone::CollisionSphere();
		vertices[i]->body = new cyclone::RigidBody();
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
	glColor3f(1, 1, 1);
    GLfloat mat[16];
    
	for ( int i = 0; i < 8; i++ )
	{
		vertices[i]->body->getGLTransform(mat);
		glPushMatrix();
		glMultMatrixf(mat);
		glScalef(vertices[i]->radius*2, vertices[i]->radius*2, vertices[i]->radius*2);
		glutSolidSphere(vertices[i]->radius, vertices[i]->radius*vertices[i]->radius*100, vertices[i]->radius*vertices[i]->radius*100);
		glPopMatrix();
	}
}

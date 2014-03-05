#include <cyclone\core.h>
#include <gl\glut.h>
#include <cyclone\pworld.h>
#include <cyclone\pfgen.h>
#include <iostream>

#include "glutBasic.h"

#define WIDTH	640
#define HEIGHT	320
#define DEPTH   300
#define PARTICLESIZE 0.03

void display();
void update();

cyclone::ParticleAnchoredSpring* spring1;
cyclone::ParticleAnchoredSpring* spring2;
cyclone::Vector3* anchorPoint1;
cyclone::Vector3* anchorPoint2;
cyclone::Particle* particle1;
cyclone::Particle* particle2;
cyclone::ParticleWorld world = cyclone::ParticleWorld(10);

void initialize(void);
void normalize(const cyclone::Vector3 &position, cyclone::Vector3 &out);

int main(int argc, char** argv) {
	// Init glut
	glutInit(&argc, argv);

	// Init application physics
	initialize();

	// Create a window
	createWindow("Springs", WIDTH, HEIGHT);

	// Set up the handler functions for glut
	glutDisplayFunc(display);	// The display callback is executed whenever GLUT
								// determines that the window should be refreshed
	glutIdleFunc(update);		// No event


	// Run the application
	// The main loop does the following things:
	// * looks at the events in the queue
	// * for each event in the queue, GLUT executes the 
	//   appropriate callback function if one is defined
	// * if no callback is defined for the event, the event is ignored
	glutMainLoop();

	delete spring1;
	delete spring2;
	delete particle1;
	delete particle2;
	delete anchorPoint1;
	delete anchorPoint2;
	// Close the application
	return 0;
}

/**
 * Called each frame to display the 3D scene. Delegates to
 * the application.
 */
void display() {

	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT);

	// Get Positions of the objects
	cyclone::Vector3 pos1, pos2, pos3, pos4;
	normalize(particle1->getPosition(), pos1);
	normalize(particle2->getPosition(), pos2);
	normalize(*anchorPoint1, pos3);
	normalize(*anchorPoint2, pos4);

	std::cout<<"POS1 "<<pos1.x<<" "<<pos1.y<<std::endl;
	std::cout<<"POS2 "<<pos2.x<<" "<<pos2.y<<std::endl;
	std::cout<<"POS3 "<<pos3.x<<" "<<pos3.y<<std::endl;
	std::cout<<"POS4 "<<pos4.x<<" "<<pos4.y<<std::endl;

	// Paint particles
	glColor3f(1, 1, 1);
	glRectf(pos1.x - PARTICLESIZE/2, pos1.y - PARTICLESIZE/2, pos1.x + PARTICLESIZE/2, pos1.y + PARTICLESIZE/2);
	glRectf(pos2.x - PARTICLESIZE/2, pos2.y - PARTICLESIZE/2, pos2.x + PARTICLESIZE/2, pos2.y + PARTICLESIZE/2);
	
	// Paint anchorpoints
	glColor3f(1, 0, 0);
	glRectf(pos3.x - PARTICLESIZE/2, pos3.y - PARTICLESIZE/2, pos3.x + PARTICLESIZE/2, pos3.y + PARTICLESIZE/2);
	glRectf(pos4.x - PARTICLESIZE/2, pos4.y - PARTICLESIZE/2, pos4.x + PARTICLESIZE/2, pos4.y + PARTICLESIZE/2);

	// Update the displayed content.
    glFlush();
    glutSwapBuffers();
}

/**
 * Called each frame to update the 3D scene. Delegates to
 * the application.
 */
void update() {
	// Clear accumulators
	world.startFrame();
	spring1->updateForce(particle1, 0.05f);
	spring2->updateForce(particle2, 0.05f);
	
	world.runPhysics(0.05f);
	
	

	glutPostRedisplay(); // Set a flag, so that the display function will be called again
}

void initialize(void) {
	// Initialize objects
	anchorPoint1 = new cyclone::Vector3(WIDTH/3, HEIGHT/3, 0);
	anchorPoint2 = new cyclone::Vector3(WIDTH/3*2, HEIGHT/3, 0);
	spring1 = new cyclone::ParticleAnchoredSpring(anchorPoint1, 1, 2);
	spring2 = new cyclone::ParticleAnchoredSpring(anchorPoint2, .5, 2);
	particle1 = new cyclone::Particle();
	particle2 = new cyclone::Particle();
	
	// Set the acceleration, so that the particles will fall down
	particle1->setAcceleration(cyclone::Vector3::HIGH_GRAVITY);
	particle2->setAcceleration(cyclone::Vector3::HIGH_GRAVITY);

	// Set the positions
	particle1->setPosition(WIDTH/3, HEIGHT/2, 0);
	particle2->setPosition(WIDTH/3*2, HEIGHT/2, 0);

	// Set the mass
	particle1->setMass(1);
	particle2->setMass(1);

	world.getParticles().push_back(particle1);
	world.getParticles().push_back(particle2);
	world.getForceRegistry().add(particle1, spring1);
	world.getForceRegistry().add(particle2, spring2);
}


/*
 * Normalize physic position to viewpoint
 */
void normalize(const cyclone::Vector3 &position, cyclone::Vector3 &out) {
	out.x = (position.x - WIDTH/2)/WIDTH;
	out.y = (position.y - HEIGHT/2)/HEIGHT;
	out.z = position.z/DEPTH;
}
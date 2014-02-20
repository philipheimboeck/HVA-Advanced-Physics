#include <cyclone\core.h>
#include <gl\glut.h>
#include <cyclone\pworld.h>
#include <cyclone\pfgen.h>

#include "glutBasic.h"

#define WIDTH	640
#define HEIGHT	320
#define DEPTH   300

void display();
void update();

cyclone::ParticleAnchoredSpring spring1;
cyclone::ParticleAnchoredSpring spring2;
cyclone::Vector3 anchorPoint1;
cyclone::Vector3 anchorPoint2;
cyclone::Particle particle1;
cyclone::Particle particle2;
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

	glColor3f(1, 0, 0);
	cyclone::Vector3 pos1;
	normalize(particle1.getPosition(),pos1);
	glRectf(-pos1.x-0.03, pos1.y + 0.03, pos1.x + 0.03, pos1.y - 0.3);
	//glRectf(particle2.getPosition().x - 5, particle2.getPosition().y - 5, particle2.getPosition().x + 5, particle2.getPosition().y + 5);
	//glRectf(-0.75f,0.75f, 0.75f, -0.75f);

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

	spring1.updateForce(&particle1, 0.05f);
	spring2.updateForce(&particle2, 0.05f);
	world.runPhysics(0.05f);

	glutPostRedisplay(); // Set a flag, so that the display function will be called again
}

void initialize(void) {
	// Initialize objects
	anchorPoint1 = cyclone::Vector3(WIDTH/3, HEIGHT/2, 0);
	anchorPoint2 = cyclone::Vector3(WIDTH/3*2, HEIGHT/2, 0);
	spring1 = cyclone::ParticleAnchoredSpring(&anchorPoint1, 1, 10);
	spring2 = cyclone::ParticleAnchoredSpring(&anchorPoint2, .5, 10);
	particle1 = cyclone::Particle();
	particle2 = cyclone::Particle();
	
	// Set the acceleration, so that the particles will fall down
	particle1.setAcceleration(cyclone::Vector3::HIGH_GRAVITY);
	particle2.setAcceleration(cyclone::Vector3::HIGH_GRAVITY);

	// Set the positions
	particle1.setPosition(WIDTH/3, HEIGHT/2, 0);
	particle2.setPosition(WIDTH/3*2, HEIGHT/2, 0);

	// Set the mass
	particle1.setMass(1);
	particle2.setMass(1);
}


/*
 * Normalize physic position to viewpoint
 */
void normalize(const cyclone::Vector3 &position, cyclone::Vector3 &out) {
	out.x = position.x/WIDTH;
	out.y = position.y/HEIGHT;
	out.z = position.z/DEPTH;
}
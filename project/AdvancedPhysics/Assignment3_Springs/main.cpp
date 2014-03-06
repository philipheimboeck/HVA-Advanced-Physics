/*
 * Assignment 3 - Springs
 * by Mathis Florian (StdNo: 500702331) and Heimböck Philip (StdNo: 500702328)
 *
 * This class simulates a slingshot, that consists of two rubber bands 
 * with different stiffnesses and a pad. Since the two rubber bands are not equal, 
 * the particle representing the pad resp. the object in it will always head to towards 
 * the stronger band.
 *
 * Weight of the Pad: 7 Grams
 * Springs: 
 *	- Restlength: 1cm
 *	- Spring constant/Stiffness (k): 0 to 10 N/m
 *	- Pulled distance (x): 11cm
 *
 * F = -k * x
 * F = -5N/m * 0.1m = -0.5 N
 * Therefor, our slingshot is pulled back with a force of -0.5 Newton per rubber band.
 * 
 *
 * By pressing enter the sling will be pulled and by pressing enter again it will be released
 * By pressing 1 or 2 you can switch between the rubber bands
 * By pressing + or - the stiffness of the selected rubber band will be increased or decreased
 */

#include <cyclone\core.h>
#include <gl\glut.h>
#include <cyclone\pworld.h>
#include <cyclone\pfgen.h>
#include <iostream>
#include <string>
#include "glutBasic.h"

#define WIDTH	640
#define HEIGHT	320
#define DEPTH   300 // for possible 3D extension
#define PARTICLESIZE 0.06

float springConstant1 = 0.5;
float springConstant2 = 0.5;
cyclone::ParticleAnchoredSpring* spring1;
cyclone::ParticleAnchoredSpring* spring2;
int selectedSpring = 1;
cyclone::Vector3* anchorPoint1;
cyclone::Vector3* anchorPoint2;
cyclone::Particle* particle1;
cyclone::ParticleWorld world = cyclone::ParticleWorld(10);

void display();
void update();
void initialize(void);
void normalize(const cyclone::Vector3 &position, cyclone::Vector3 &out);
void keyPress(unsigned char key, int x, int y);
bool pull = false;

/*
 * Application entry point
 * This method will initialize the basic settings 
 * of our physic application and start the cyclone engine.
 */
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
	glutKeyboardFunc(keyPress);


	// Run the application
	// The main loop does the following things:
	// * looks at the events in the queue
	// * for each event in the queue, GLUT executes the 
	//   appropriate callback function if one is defined
	// * if no callback is defined for the event, the event is ignored
	glutMainLoop();

	// clean up the world
	delete spring1;
	delete spring2;
	delete particle1;
	delete anchorPoint1;
	delete anchorPoint2;
	// Close the application
	return 0;
}

/**
 * Display
 * This method is called every frame in order to 
 * repaint our scene with the applied physics
 * and calculated positions of the particles.
 */
void display() {

	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT);

	// Get Positions of the objects
	cyclone::Vector3 pos1, pos3, pos4;

	// Normalize particle and anchor points in order to 
	// correctly draw them to our scene
	normalize(particle1->getPosition(), pos1);
	normalize(*anchorPoint1, pos3);
	normalize(*anchorPoint2, pos4);

	// Paint particles
	glColor3f(1, 0, 0);
	glRectf(pos1.x - PARTICLESIZE/2, pos1.y - PARTICLESIZE/2, pos1.x + PARTICLESIZE/2, pos1.y + PARTICLESIZE/2);
	
	// Paint anchorpoints
	if ( selectedSpring == 1 ) glColor3f(0, 1, 0);
	else glColor3f(1, 1, 1);
	glRectf(pos3.x - PARTICLESIZE/2, pos3.y - PARTICLESIZE/2, pos3.x + PARTICLESIZE/2, pos3.y + PARTICLESIZE/2);

	if ( selectedSpring == 2 ) glColor3f(0, 1, 0);
	else glColor3f(1, 1, 1);
	glRectf(pos4.x - PARTICLESIZE/2, pos4.y - PARTICLESIZE/2, pos4.x + PARTICLESIZE/2, pos4.y + PARTICLESIZE/2);

	glColor3f(1, 1, 1);

	// Write strength text to each fixed anchorpoint
	glRasterPos2f(pos3.x, pos3.y - .1);
	std::string strengthText = std::to_string((long long)(springConstant1*10));
	for ( size_t i = 0; i < strengthText.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, strengthText.at(i));
	}
	
	glRasterPos2f(pos4.x, pos4.y - .1);
	strengthText = std::to_string((long long)(springConstant2*10));
	for ( size_t i = 0; i < strengthText.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, strengthText.at(i));
	}
	
	// Draw rubber bands
	glBegin(GL_LINES);
	glVertex3f(pos3.x, pos3.y, 0);
	glVertex3f(pos1.x, pos1.y, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(pos4.x, pos4.y, 0);
	glVertex3f(pos1.x, pos1.y, 0);
	glEnd();

	// Draw description to help 
	glRasterPos2f(-.99, .9);
	std::string text = "Select the left rubber band with [0], the right rubber band with [1]."; 
	for ( size_t i = 0; i < text.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text.at(i));
	}
	glRasterPos2f(-.99, .8);
	text = "Increase the strength with [+] and decrease it with [-].";
	for ( size_t i = 0; i < text.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text.at(i));
	}
	glRasterPos2f(-.99, .7);
	text = "Hit [ENTER] key to draw sling and again to release the shot.";
	for ( size_t i = 0; i < text.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text.at(i));
	}
	
	// Update the displayed content.
    glFlush();
    glutSwapBuffers();
}

/**
 * Update
 * Hold on physic engine whenever we want to pull our slingshot
 */
void update() {
	// Clear accumulators
	world.startFrame();
	
	// Pause the physic when we "pull" the particle
	if ( !pull ) world.runPhysics(0.05f);
	
	// Set a flag, so that the display function will be called again
	glutPostRedisplay(); 
}

/*
 * keyPress
 * This method handles the user inputs in order
 * to update parameters of our slingshot like
 * stiffness of each rubber band.
 */
void keyPress(unsigned char key, int x, int y)
{
	// Press enter to draw and again to release the slingshot
	switch (key)
		{
		case 13: // enter
			
			// First we tried to add another force to the particle that pulls it backwards (spanning the sling)
			// But it was not possible to remove the force because that method is missing
			// Therefore we now set the position and pause the physics simulation (yeah it is a bad hack but at least it works)
			if ( !pull ) {
				particle1->setPosition(WIDTH/2, 50, 0);
				//world.getForceRegistry().add(particle1, spring3);
			} else {
				//world.getForceRegistry().remove(particle1, spring3); 
			}

			pull = !pull;
			break;
		case 43: // +
			if ( selectedSpring == 1) {
				// Increase spring constant of left rubber band
				springConstant1 = min(1, springConstant1 + 0.1);
				spring1->init(anchorPoint1, springConstant1, 0);
			} else { 
				// Increase spring constant of right rubber band
				springConstant2 = min(1, springConstant2 + 0.1);
				spring2->init(anchorPoint2, springConstant2, 0);
			}
			break;
		case 45: // -
			if ( selectedSpring == 1) {
				// Decrease spring constant of left rubber band
				springConstant1 = max(0, springConstant1 - 0.1);
				spring1->init(anchorPoint1, springConstant1, 0);
			} else {
				// Decrease spring constant of right rubber band
				springConstant2 = max(0, springConstant2 - 0.1);
				spring2->init(anchorPoint2, springConstant2, 0);
			}
			break;
		case 49: // 1
			selectedSpring = 1;
			break;
		case 50: // 2
			selectedSpring = 2;
			break;
		default:
			std::cout<<(int)key<<std::endl;
	}
}

/*
 * Initialize our physical world
 * by creating two anchor points and two springs for each rubber band side
 */
void initialize(void) {
	// Initialize objects
	anchorPoint1 = new cyclone::Vector3(WIDTH/3, HEIGHT/2, 0);
	anchorPoint2 = new cyclone::Vector3(WIDTH/3*2, HEIGHT/2, 0);
	// Set spring with given spring constant and rest length of 1cm
	spring1 = new cyclone::ParticleAnchoredSpring(anchorPoint1, springConstant1, 10);
	spring2 = new cyclone::ParticleAnchoredSpring(anchorPoint2, springConstant2, 10);
	particle1 = new cyclone::Particle();

	// Set the positions
	particle1->setPosition(WIDTH/2, HEIGHT/2, 0);

	// Set Velocity and Damping Factor
	particle1->setVelocity(0,0,0);
	particle1->setDamping(0.9f); // you need to set the damping factor otherwise the velocity calculations lead to a IND

	// Set the mass to 7 Gramms
	particle1->setMass(7);

	// Add Particle to the World (insert at the end of particle list)
	world.getParticles().push_back(particle1);

	// Add Force of Particle 1 to Spring 1 and Spring 2 to the force registry
	world.getForceRegistry().add(particle1, spring1);
	world.getForceRegistry().add(particle1, spring2);
}

/*
 * Normalize physical position to viewpoint
 */
void normalize(const cyclone::Vector3 &position, cyclone::Vector3 &out) 
{
	out.x = (position.x - WIDTH/2)/(WIDTH/2);
	out.y = (position.y - HEIGHT/2)/(HEIGHT/2);
	out.z = position.z/DEPTH;
}
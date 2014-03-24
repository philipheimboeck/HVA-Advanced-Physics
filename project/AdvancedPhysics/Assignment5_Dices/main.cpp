/*
* Assignment 5 - Dices
* by Mathis Florian (StdNo: 500702331) and Heimböck Philip (StdNo: 500702328)
*
*/

#include <cyclone\core.h>
#include <cyclone\pworld.h>
#include <cyclone\pfgen.h>
#include <cyclone\collide_fine.h>
#include <gl\glut.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
#include <time.h>

#include "glutBasic.h"

// Define Wall of Boxes Settings
#define WIDTH	1028
#define HEIGHT	640
#define MAXCONTACTS 256

// Callback function that draws everything on the screen
void display();
// Callback function for physic calculations
void update();
// Callback function that will handle the inputs
void keyPress(unsigned char key, int x, int y);
// Initializing the boxes (will also be called by Pressing N for New)
void initialize();
// Reset the simulation with the same masses
void reset();

// Create the Contact Resolve and contacts array
cyclone::ContactResolver resolver = cyclone::ContactResolver(MAXCONTACTS * 8);
cyclone::Contact contacts[MAXCONTACTS];
cyclone::CollisionData cData;

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

	// Initialize random
	std::srand(time(NULL));

	// Create a window
	createWindow("Wall of Boxes", WIDTH, HEIGHT);

	// Initialize graphics
	GLfloat lightAmbient[] = {0.8f,0.8f,0.8f,1.0f};
	GLfloat lightDiffuse[] = {0.9f,0.95f,1.0f,1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glEnable(GL_LIGHT0);
	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)WIDTH/(double)HEIGHT, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);

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

	// Close the application
	return 0;
}

void display() {
	const static GLfloat lightPosition[] = {-1,1,0,0};

	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(NUMBEROFBOXES_WIDTH*BOXSIZE + 50, 64, -20,  NUMBEROFBOXES_HEIGHT*BOXSIZE, 5.0, 50.0,  0.0, 1.0, 0.0);

	

	// Update the displayed content.
	glFlush();
	glutSwapBuffers();
}

void update()
{

	// Set a flag, so that the display function will be called again
	glutPostRedisplay();
}

/**
 * KeyPress Method
 * Interact to the user input 
 */
void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	default:
		std::cout<<(int)key<<std::endl;
	}
}

/*
 * Initialize the wall of boxes
 */
void initialize()
{
}

/*
 * Reset the world to start values
 */
void reset()
{
}
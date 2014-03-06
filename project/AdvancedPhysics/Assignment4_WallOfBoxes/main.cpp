/*
 * Assignment 4 - Wall of Boxes
 * by Mathis Florian (StdNo: 500702331) and Heimböck Philip (StdNo: 500702328)
 *
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

void display();
void update();
void initialize(void);
void keyPress(unsigned char key, int x, int y);

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
	createWindow("Wall of Boxes", WIDTH, HEIGHT);

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
	// Clear the scene
	glClear(GL_COLOR_BUFFER_BIT);

}

void update() {
}

void initialize(void) {
}

void keyPress(unsigned char key, int x, int y) {
}
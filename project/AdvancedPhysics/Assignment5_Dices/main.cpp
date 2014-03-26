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
#include "Dice.h"

// Define Wall of Boxes Settings
#define WIDTH	1028
#define HEIGHT	640
#define MAXCONTACTS 1024
#define NUMBEROFDICES 3

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

Dice* dices[3];

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
	gluLookAt( -30.0, 15.0, -5.0, 0.0, 15.0, 0.0, 0.0, 10.0, 0.0 );

	// Print the Floor
	glColor3f(0.7, 0.8, 0.8);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0); glTexCoord2f(0,0); glNormal3f(0,1,0);
    glVertex3f(300,0,0); glTexCoord2f(1,0); glNormal3f(0,1,0);
    glVertex3f(300,0,300); glTexCoord2f(1.0,1.0); glNormal3f(0,1,0);
    glVertex3f(0,0,300); glTexCoord2f(0.0,1.0); glNormal3f(0,1,0);
	glEnd();

	// Print all axes - X
	glColor3f(0,0,0);
	glRasterPos3f(2, 0, 0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'X');
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glEnd();

	// Axis Y
	glRasterPos3f(0, 2, 0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y');
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glEnd();

	// Axis Z
	glRasterPos3f(0, 0, 2);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Z');
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();

	for ( int i = 0; i < NUMBEROFDICES; i++ ) 
	{
		dices[i]->render();
	}

	// Update the displayed content.
	glFlush();
	glutSwapBuffers();
}

void update()
{
	float duration = 0.05f;
	for ( int i = 0; i < NUMBEROFDICES; i++ ) 
	{
		dices[i]->integrate(duration);
	}

	// Create the floor plane contact
    cyclone::CollisionPlane plane;
    plane.direction = cyclone::Vector3(0,1,0);
    plane.offset = 0;

	// Setting the collision data
    cData.reset(MAXCONTACTS);
    cData.friction = (cyclone::real)0.9;
    cData.restitution = (cyclone::real)0.1;
    cData.tolerance = (cyclone::real)0.1;

	// Check the each Box to Floor collision
	//cyclone::CollisionDetector::sphereAndHalfSpace(dices[0], plane, &cData);
	for ( int i = 0; i < NUMBEROFDICES; i++ ) 
	{
		dices[i]->createContactsPlane(&plane, &cData);

		for ( int j = i; j < NUMBEROFDICES; j++)
		{
			dices[i]->createContactsDice(dices[j], &cData);
		}
	}

	// Resolve detected contacts
    resolver.resolveContacts(
        cData.contactArray,
        cData.contactCount,
        duration
    );

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
	case 'r': case 'R':
		// Reset
		std::cout<<"Reset"<<std::endl;
		reset();
		break;

	default:
		std::cout<<(int)key<<std::endl;
	}
}

/*
 * Initialize the wall of boxes
 */
void initialize()
{
	dices[0] = new Dice(2, 0, 0, 0);
	dices[1] = new Dice(2, 15, 15, 15);
	dices[2] = new Dice(2, 15, 30, 14);

	// Set the contact array to store our box/floor contacts
	cData.contactArray = contacts;
}

/*
 * Reset the world to start values
 */
void reset()
{
	for ( int i = 0; i < NUMBEROFDICES; i++ )
	{
		delete dices[i];
	}
	initialize();
}
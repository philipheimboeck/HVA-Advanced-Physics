/*
* Assignment 4 - Wall of Boxes
* by Mathis Florian (StdNo: 500702331) and Heimb�ck Philip (StdNo: 500702328)
*
*/

#include <cyclone\core.h>
#include <gl\glut.h>
#include <cyclone\pworld.h>
#include <cyclone\pfgen.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

#include "glutBasic.h"
#include "Box.h"

#define WIDTH	640
#define HEIGHT	320
#define DEPTH   300 // for possible 3D extension
#define NUMBEROFBOXES 2

void display();
void update();
void keyPress(unsigned char key, int x, int y);
void initialize();
void initializeBox(Box* box, cyclone::Vector3 position, cyclone::real mass);
void launchBox(void);

Box boxes[NUMBEROFBOXES][NUMBEROFBOXES];
Box missileBox;

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
	gluLookAt(-25.0, 8.0, 5.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

	// Print all axes
	glColor3f(0,0,0);
	glRasterPos3f(2, 0, 0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'X');
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glEnd();

	glRasterPos3f(0, 2, 0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y');
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glEnd();

	glRasterPos3f(0, 0, 2);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Z');
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();

	// Render all Boxes
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1,0,0);
	missileBox.render();
	for ( int i = 0; i < NUMBEROFBOXES; i++ )
	{
		for ( int j = 0; j < NUMBEROFBOXES; j++ )
		{
			boxes[i][j].render();
		}
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// Update the displayed content.
	glFlush();
	glutSwapBuffers();
}

void update()
{
	missileBox.integrate(0.05f);
	missileBox.calculateInternals();
	for ( int i = 0; i < NUMBEROFBOXES; i++ )
	{
		for ( int j = 0; j < NUMBEROFBOXES; j++ )
		{
			boxes[i][j].integrate(0.05f);
			boxes[i][j].calculateInternals();
		}
	}

	// Set a flag, so that the display function will be called again
	glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r' || 'R':
		// Reset
		initialize();
	case ' ':
		// Launch box
		launchBox();
		break;
	default:
		std::cout<<(int)key<<std::endl;
	}
}

void initialize() 
{
	initializeBox(&missileBox, cyclone::Vector3(0, 0, 10.0f), 3);

	for ( int i = 0; i < NUMBEROFBOXES; i++ )
	{
		for ( int j = 0; j < NUMBEROFBOXES; j++ ) 
		{
			cyclone::Vector3 position(i * 2 * boxes[i][j].halfSize.x, j * 2 * 
				boxes[i][j].halfSize.y, 50.0f);

			cyclone::real mass = 3;

			initializeBox(&boxes[i][j], position, mass);
		}
	}
}

void initializeBox(Box* box, cyclone::Vector3 position, cyclone::real mass)
{
	box->halfSize = cyclone::Vector3(3, 3, 3);
	box->setPosition(position);
	box->setDamping(0.95f, 0.8f);
	box->setOrientation(1, 0, 0, 0);
	box->setMass(mass);

	box->calculateInertia();


	// body->clearAccumulators();
	// body->setAcceleration(0,-10.0f,0);

	// body->setCanSleep(false);
	// body->setAwake();

	box->recalculate();
}

void launchBox(void)
{
	std::cout<<"Box launched"<<std::endl;
}

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(), 
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
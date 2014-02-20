#include <cyclone\core.h>
#include <gl\glut.h>

#include "glutBasic.h"

#define WIDTH	640
#define HEIGHT	320

void display();
void update();

int main(int argc, char** argv) {
	// Init glut
	glutInit(&argc, argv);

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

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);	// Delimit the vertices of a primitive or a group of like primitives
	glVertex3i(1, 1, 0);	// Specify x, y, z, and w coordinates of a vertex. Not all parameters are present in all forms of the command.
    glVertex3i(WIDTH - 1, HEIGHT - 1, 0);
    glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1,1,1);
	glVertex3f(-1,-1,0);
	glVertex3f(1,-1,0);
	glVertex3f(1,1,0);
	glEnd();

	glColor3f(.3, 0, 0);
	glRectf(-.5, -.5, .5, .5);

	// Update the displayed content.
    glFlush();
    glutSwapBuffers();
}

/**
 * Called each frame to update the 3D scene. Delegates to
 * the application.
 */
void update() {
	glutPostRedisplay(); // Set a flag, so that the display function will be called again
}
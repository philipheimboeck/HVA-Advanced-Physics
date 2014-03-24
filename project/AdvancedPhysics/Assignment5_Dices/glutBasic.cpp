#include "glutBasic.h"
#include <gl\glut.h>
#include <string>

void createWindow(const char* title, int width, int height) 
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);
}

void drawHudText(std::string text, int width, int height, int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_DEPTH_BUFFER_BIT);

	glRasterPos2f(x, y);
	for ( size_t i = 0; i < text.length(); i++ ) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text.at(i));
	}

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

}
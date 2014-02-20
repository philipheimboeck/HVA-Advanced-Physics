#include "glutBasic.h"
#include <gl\glut.h>

void createWindow(const char* title, int width, int height) 
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);
}
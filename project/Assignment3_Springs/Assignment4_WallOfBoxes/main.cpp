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
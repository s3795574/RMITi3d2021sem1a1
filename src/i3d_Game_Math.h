#pragma once
#include "i3d_Game_Objects.h"

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

//Given x and y in a 2D cartesian coordinate, return the length form origin. NOTE: return value is a scalar
float getLength2D(float x, float y);
//normalize vector to unit vector
void normalizing(Vector2D* direction);
//Get angel in degree
float getAngleInDegree(Vector2D* direction);
//calcualte the new position with the given length
void update_position(Vector2D* direction, Vector2D* position, float degree,float length);
//update the direction according to the current angle
void update_direction(Vector2D* direction, float degree);
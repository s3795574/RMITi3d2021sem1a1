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
//normalize vector to unit vector. direction is a point that the object towards from origin.
void normalizing(Vector2D* direction, float originX, float originY);
//Get angel in degree
float getAngleInDegree(Vector2D* direction);
//calcualte the new position with the given length
void update_position(Vector2D* direction, Vector2D* position, float degree,float movement);
//changing the direction to a given angle
void update_direction(Vector2D* direction, float degree);
//Ratating the direction with the given angle
void rotation(Vector2D* direction, float degree);
float getDistance(Vector2D* point1, Vector2D* point2);
//Get a random raito between the given parameter. e.g. 30 with return a value between -0.3 and 0.3.
float random_raito(int percentage);
float get_normal_x(float x1, float x2, float distance);
float get_normal_y(float y1, float y2, float distance);
float get_dot_product(float x1, float y1, float x2, float y2);

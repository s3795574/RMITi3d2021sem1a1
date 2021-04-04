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

//Rendering the ship on the screen. width and height is the size of the screen.
extern void render_spaceShip(int width, int height, Spaceship* ship);
//Rendering the arana on the screen. width and height is the size of the screen.
extern void render_arena(int width, int height, Color* color);
//Rednering a circle. The radius should between 0 and 1. width and height is the size of the screen.
extern void render_circle(int width, int height, GLfloat radius, Color* color);
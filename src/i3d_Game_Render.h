#pragma once
#include "i3d_Game_Objects.h"
#include "i3d_Game_Particle.h"
#include "i3d_Game_Text_Display.h"

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
extern void render_spaceShip(int width, int height, Spaceship* ship, float ship_scale_size);
//Rendering the arana on the screen. width and height is the size of the screen.
extern void render_arena(Arena* arena);
//Rending a circle. The radius should between 0 and 1. width and height is the size of the screen.
extern void render_circle(int width, int height, Asteroid* asteroid, GLfloat radius, Color* color, float circle_scale_size);
//Rending bullet
extern void render_bullet(Game_Window* window, Spaceship* ship, Bullet* bullet);
//Rending puff
extern void render_particle(Game_Window* window, Spaceship* ship, Particle* particle);
//Redning game time on the top-right
extern void render_game_time(Game_Log* game_log);
//Rending game score on the top-left
extern void render_game_score(Game_Log* game_log);
//Rending end game info
extern void render_end_game_info(Game_Log* game_log);
//Rending explosion
extern void render_explosion(Game_Window* window, Particle_v2* particle, int array_size, int scale_size);
extern void render_circle_test(int width, int height);
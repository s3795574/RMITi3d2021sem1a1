#include <stdlib.h>
#include <stdio.h>
#include "i3d_Game_Render.h"
#include "i3d_Game_Objects.h"
#include "i3d_Game_Math.h"

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

#define KEY_ESC 27
#define START_POS_RATIO 0.2 // How far the ship is away from the bottom left of the screen
#define SHIP_VELOCITY 20
#define SHIP_AV 90 // angular velocity, degree per second
#define KEY_PRESSED 1
#define KEY_RELEASED 0

//variables
int g_screen_width = 0;
int g_screen_height = 0;
float g_last_time = 0.0;
//Objects
Vector2D shipDirection;
Vector2D shipPostion;
Spaceship ship;
Arena arena;
Keyboard keyboard;
Color arena_color;

void on_reshape(int width, int height) {
	fprintf(stderr, "on_reshape(%d, %d)\n", width, height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, 1.0, -1.0);
	//set current window size
	g_screen_width = width;
	g_screen_height = height;
	//init direction of the ship
	vec2d_t_init(&shipDirection, g_screen_width, g_screen_width);
	normalizing(&shipDirection);
	//init postion of the ship
	vec2d_t_init(&shipPostion, g_screen_width * START_POS_RATIO, g_screen_height * START_POS_RATIO);
	ship2d_t_init(&ship, &shipDirection, &shipPostion, SHIP_VELOCITY, SHIP_AV);
	ship.radius = 0.05 * g_screen_width;//temporary solution, set the bounding circle of the ship
	//init keyboard stats
	keyboard_init(&keyboard);
	//init arena
	set_arena(&arena, width * 0.05, width * 0.95, height * 0.95, height * 0.05);
	fprintf(stderr, "unit vector(%f, %f)\n", shipDirection.x, shipDirection.y);
	fprintf(stderr, "degree(%f)\n", getAngleInDegree(&shipDirection));
}
void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	render_arena(g_screen_width, g_screen_height,&arena_color);
	render_spaceShip(g_screen_width, g_screen_width, &ship);
	render_circle(g_screen_width, g_screen_height, 1, &arena_color);

	/* Always check for errors! */
	int err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("display: %s\n", gluErrorString(err));

	glutSwapBuffers();
}

//changing the keyboard stats
void on_keyboard_press(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'w':
		keyboard.w = KEY_PRESSED;
		break;
	case 'a':
		keyboard.a = KEY_PRESSED;
		break;
	case 'd':
		keyboard.d = KEY_PRESSED;
		break;
	default:
		break;
	}
}
//changing the keyboard stats
void on_keyboard_release(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		keyboard.w = KEY_RELEASED;
		break;
	case 'a':
		keyboard.a = KEY_RELEASED;
		break;
	case 'd':
		keyboard.d = KEY_RELEASED;
		break;
	default:
		break;
	}
}

void on_idle()
{
	float cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	float dt = cur_time - g_last_time;

	glutKeyboardFunc(on_keyboard_press);
	glutKeyboardUpFunc(on_keyboard_release);
	//checking if the ship crash on the wall
	if (hit_wall(&ship, &arena) == 0) {
		arena_color.R = 1;
		arena_color.G = 1;
		arena_color.B = 1;
	}
	else if(hit_wall(&ship, &arena) == 1){
		arena_color.R = 1;
		arena_color.G = 0;
		arena_color.B = 0;
		fprintf(stderr, "ship(%f,%f)\n", ship.position->x, ship.position->y);
		fprintf(stderr, "wall(%f,%f)\n", arena.left, arena.bottom);
	}
	else if (hit_wall(&ship, &arena) == 2) {
		reset_ship(&ship, g_screen_width * START_POS_RATIO, g_screen_height * START_POS_RATIO);
	}


	//Movement of the ship
	if (keyboard.w == 1) {
		update_ship_position(&ship, SHIP_VELOCITY * dt);
		fprintf(stderr, "w(%f,%f)\n", ship.direction->x,ship.direction->y);
	}
	if (keyboard.a == 1) {
		update_ship_angle(&ship, SHIP_AV * dt);
		fprintf(stderr, "a(%f,%f,%f)\n", ship.current_degree, ship.direction->x, ship.direction->y);
	}
	if (keyboard.d == 1) {
		update_ship_angle(&ship, -(SHIP_AV * dt));
		fprintf(stderr, "d(%f,%f,%f)\n", ship.current_degree, ship.direction->x, ship.direction->y);
	}

	g_last_time = cur_time;
	glutPostRedisplay();
}

void init(int* argc, char** argv)
{
	color_init(&arena_color);
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Assignment 1 - s3795574");
	//glutFullScreen();
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);
	glutIdleFunc(on_idle);
}

int main(int argc, char** argv)
{
	init(&argc, argv);
	glutMainLoop();
	return EXIT_SUCCESS;
}

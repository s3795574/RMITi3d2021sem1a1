#include <stdlib.h>
#include <stdio.h>
#include "i3d_Game_Render.h"
#include "i3d_Game_Objects.h"
#include "i3d_Game_Math.h"
#include "i3d_Game_Particle.h"
#include "i3d_config.h"

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

//variables
int g_screen_width = 0;
int g_screen_height = 0;
float g_last_time = 0.0;
float g_asteroid_time = 0.0;
float g_particle_hold = 0.0f;
float cur_time = 0.0f;
float dt = 0;
int current_number_of_asteroid = 1;
int puff_index = 0;
char* t;
char minutes[100];
char seconds[100];
char score[100];
//Objects
Game_Window window;
//ship
Vector2D shipDirection;
Vector2D shipPostion;
Spaceship ship;
Color ship_outline_color;
Color ship_filling_color;
//arena
Arena arena;
//asteroids
Asteroid asteroids[NUMBER_OF_ASTEROID];
Vector2D asteroid_Postions[NUMBER_OF_ASTEROID];
Vector2D asteroid_directions[NUMBER_OF_ASTEROID];
//keyboard
Keyboard keyboard;
//color
Color arena_color;
Color asteroids_color;
Color puff_color;
//bulllets
Bullet bullets[NUMBER_OF_BULLETS];
Vector2D bullets_Postion[NUMBER_OF_BULLETS];
Vector2D bullets_direction[NUMBER_OF_BULLETS];
//puff
Particle puff[NUMBER_OF_PARTICLE_IN_PUFF];
Vector2D puff_Postion[NUMBER_OF_PARTICLE_IN_PUFF];
Vector2D puff_direction[NUMBER_OF_PARTICLE_IN_PUFF];
//Game log
Game_Log game_log;

void on_reshape(int width, int height) {
	glViewport(0, 0, g_screen_width, g_screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect_ratio = g_screen_width / g_screen_height;
	glOrtho(0.0, g_screen_width, 0.0, g_screen_height, 1.0, -1.0);
	//set current window size
	//window_init(&window, g_screen_width, g_screen_height, FULL_SCREEN);
	
}
void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	render_game_score(&game_log);
	render_game_time(&game_log);

	render_arena(&arena);
	if (ship.active == 1) {
		render_spaceShip(window.width, window.height, &ship, SHIP_SCALE_SIZE);
	}
	else
	{
		render_end_game_info(&game_log);
	}

	for (int i = 0; i < current_number_of_asteroid; i++) {
		if (asteroids[i].active == 1) {
			render_circle(window.width, window.height, &asteroids[i], asteroids[i].radius, &asteroids_color, ASTEROID_SCALE_SIZE);
		}	
	}

	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		if (bullets[i].fired == 1) {
			render_bullet(&window, &ship, &bullets[i]);
		}
	}

	for (int i = 0; i < NUMBER_OF_PARTICLE_IN_PUFF; i++) {
		if (puff[i].active == 1) {
			render_particle(&window, &ship, &puff[i]);
		}
	}

	

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
	case KEY_ESC:
	//case 'q':
		exit(EXIT_SUCCESS);
		break;
	case KEY_FORWARD:
	//case 'w':
		keyboard.forward = KEY_PRESSED;
		break;
	case KEY_ANIT_CLOCKWISE:
	//case 'a':
		keyboard.anti_clockwise = KEY_PRESSED;
		break;
	case KEY_CLOCKWISE:
	//case 'd':
		keyboard.clockwise = KEY_PRESSED;
		break;
	default:
		if (ship.active == 0) {
			reset_ship(&ship, &window, window.width * START_POS_RATIO + arena.left, window.height * START_POS_RATIO + arena.bottom);
			//reset asteroids
			for (int i = 0; i < NUMBER_OF_ASTEROID; i++) {
				launch_asteroid(&ship, &asteroids[i], &asteroid_directions[i], &asteroid_Postions[i],
					window.width, window.height, ASTEROID_VELOCITY, ASTEROID_ANGULAR_VELOCITY, ASTEROID_SCALE_SIZE);
			}
			current_number_of_asteroid = 1;
			//reset game score
			game_log_init(&game_log, g_screen_width, g_screen_height, minutes, seconds, score);
		}
		break;
	}
}
//changing the keyboard stats
void on_keyboard_release(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		keyboard.forward = KEY_RELEASED;
		break;
	case 'a':
		keyboard.anti_clockwise = KEY_RELEASED;
		break;
	case 'd':
		keyboard.clockwise = KEY_RELEASED;
		break;
	default:
		break;
	}
}

void on_mouse_button(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
			if (bullets[i].fired == 0) {
				bullets[i].fired = 1;
				break;
			}
		}
		
	}
}

void on_idle()
{
	cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	update_game_time(&game_log,cur_time, minutes, seconds);
	dt = cur_time - g_last_time;
	
	//increase the number of asteroid over time
	if (current_number_of_asteroid < NUMBER_OF_ASTEROID && cur_time - g_asteroid_time >= WAVE_INTERVAL) {
		current_number_of_asteroid++;
		g_asteroid_time = cur_time;
	}
	//checking if the ship crash on the wall
	if (hit_wall(&ship, &arena, WARNING_RAITO) == 0) {
		arena_color.R = 1;
		arena_color.G = 1;
		arena_color.B = 1;
	}
	else if(hit_wall(&ship, &arena, WARNING_RAITO) == 1){
		arena_color.R = 1;
		arena_color.G = 0;
		arena_color.B = 0;
	}
	else if (hit_wall(&ship, &arena, WARNING_RAITO) == 2) {
		//ship hit the wall
	}

	//Movement of the ship
	if (keyboard.forward == 1) {
		update_ship_position(&ship,ship.current_degree, SHIP_VELOCITY * dt);
		g_particle_hold += dt;
		if (g_particle_hold > 0.15) {	
			particle_release(&ship,&puff[puff_index], g_screen_width * 0.02f);
			g_particle_hold = 0;
			puff_index++;
			if (puff_index >= NUMBER_OF_PARTICLE_IN_PUFF) {
				puff_index = 0;
			}
		}		
	}
	if (keyboard.anti_clockwise == 1) {
		update_ship_angle(&ship, SHIP_AV * dt);
		//fprintf(stderr, "a(%f,%f,%f)\n", ship.current_degree, ship.direction->x, ship.direction->y);
	}
	if (keyboard.clockwise == 1) {
		update_ship_angle(&ship, -(SHIP_AV * dt));
		//fprintf(stderr, "d(%f,%f,%f)\n", ship.current_degree, ship.direction->x, ship.direction->y);
	}

	//Movement of an astroid
	for (int i = 0; i < current_number_of_asteroid; i++) {
		update_asteroid_position(&asteroids[i], window.width, window.height, asteroids[i].velocity * dt);
		update_asteroid_angle(&asteroids[i], ASTEROID_ANGULAR_VELOCITY * dt);
		//checking collision
		spaceship_asteroid_collision(&ship, &asteroids[i]);
	}

	//Update bullets status
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		if (bullets[i].fired == 1) {
			//calculate the movement of bullets
			update_bullet_position(&bullets[i], bullets[i].velocity * dt);
		}
		else {
			//if bullet was not fired, keep updating bullet with ship position and direction
			bullets[i].position->x = ship.position->x;
			bullets[i].position->y = ship.position->y;
			bullets[i].direction->x = ship.direction->x;
			bullets[i].direction->y = ship.direction->y;
		}
	}	
	//checking collision between arena and bullets
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		bullet_arena_collision(&bullets[i] , &arena);
	}
	int is_collision;
	//checking collision between asteroids and bullets
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		for (int j = 0; j < NUMBER_OF_ASTEROID; j++) {
			is_collision = bullet_asteroid_collision(&bullets[i], &asteroids[j]);
			if (is_collision == 2) {
				update_game_score(&game_log);
			}
		}
	}
	//updating particle
	for (int i = 0; i < NUMBER_OF_PARTICLE_IN_PUFF; i++) {
		if (puff[i].active == 0) {
			puff[i].radius = g_screen_width * 0.01;
			puff[i].lifespan = PARTICLE_LIFESPAN;
		}
		else {
			puff[i].lifespan -= dt;
			if (puff[i].lifespan <= 0) {
				puff[i].active = 0;
			}
			puff[i].radius = puff[i].radius * (1 - dt * PARTICLE_DECAY);
		}
	}

	g_last_time = cur_time;
	glutPostRedisplay();
}

void init_game() {
	//init game window
	window_init(&window, g_screen_width, g_screen_height, FULL_SCREEN);
	//init game log
	game_log_init(&game_log, g_screen_width, g_screen_height, minutes, seconds, score);
	//init colors
	color_init(&ship_outline_color,SHIP_OUTLINE_R, SHIP_OUTLINE_G, SHIP_OUTLINE_B);
	color_init(&ship_filling_color, SHIP_FILLING_R, SHIP_FILLING_G, SHIP_FILLING_B);
	color_init(&arena_color,ARENA_COLOR_R, ARENA_COLOR_G, ARENA_COLOR_B);
	color_init(&asteroids_color, ASTEROID_COLOR_R,ASTEROID_COLOR_G, ASTEROID_COLOR_B);
	color_init(&puff_color, 1, 0 , 0);
	//init arena
	arena_init(&arena, &arena_color,g_screen_width,g_screen_height,ARENA_SCALE_SIZE);
	//init a point the ship towards, by default, it is the top-right corner
	vec2d_t_init(&shipDirection, window.width, window.height);
	//init postion of the ship
	vec2d_t_init(&shipPostion, window.width * START_POS_RATIO + arena.left, window.height * START_POS_RATIO + arena.bottom);
	//calculate the direction of the ship
	normalizing(&shipDirection, window.width * START_POS_RATIO, window.height * START_POS_RATIO);
	//init the ship
	ship2d_t_init(&ship, &shipDirection, &shipPostion, &ship_outline_color, &ship_filling_color,
			SHIP_VELOCITY, SHIP_AV, SHIP_SCALE_SIZE * g_screen_width);
	//init asteroids
	for (int i = 0; i < NUMBER_OF_ASTEROID; i++) {
		launch_asteroid(&ship, &asteroids[i], &asteroid_directions[i], &asteroid_Postions[i],
			window.width, window.height, ASTEROID_VELOCITY, ASTEROID_ANGULAR_VELOCITY, ASTEROID_SCALE_SIZE);
	}
	//init bullets
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		bullet_init(&bullets[i], &ship, &bullets_direction[i], &bullets_Postion[i], BULLET_VELOCITY);
		bullets[i].fired = 0;
	}

	//init particle
	for (int i = 0; i < NUMBER_OF_PARTICLE_IN_PUFF; i++) {
		puff_init(&ship, &puff[i], &puff_Postion[i], &window, &puff_color, g_screen_width * 0.01, PARTICLE_LIFESPAN, PARTICLE_SIZE);
	}
	//init keyboard stats
	keyboard_init(&keyboard);
}

void init(int* argc, char** argv)
{

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitWindowPosition(300, 300);
	//glutInitWindowSize(window.width, window.height);
	glutCreateWindow("Assignment 1 - s3795574");
	glutFullScreen();
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);

	glutMouseFunc(on_mouse_button);
	glutKeyboardFunc(on_keyboard_press);
	glutKeyboardUpFunc(on_keyboard_release);

	glutIdleFunc(on_idle);
	g_screen_width = glutGet(GLUT_SCREEN_WIDTH);
	g_screen_height = glutGet(GLUT_SCREEN_HEIGHT);
	init_game();
}

int main(int argc, char** argv)
{
	srand(time(NULL)); //seed the random generator
	init(&argc, argv);
	glutMainLoop();
	return EXIT_SUCCESS;
}

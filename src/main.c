#include <stdlib.h>
#include <stdio.h>
#include "i3d_Game_Render.h"
#include "i3d_Game_Objects.h"
#include "i3d_Game_Math.h"
#include "i3d_Game_Particle.h"
#include "i3d_config.h"
#include "i3d_Game_Bounce.h"
#include "i3d_Game_Bonus_Features.h"

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
float g_asteroid_time = 0.0; // compare with the WAVE_INTERVAL to decide if a new asteroid need to be added into the game.
float g_particle_hold = 0.0f; // Time interval between each partical that being released from the rear of the ship
float cur_time = 0.0f;
float dt = 0;
int current_number_of_asteroid = 0;
int current_number_of_splited_asteroid = 0;
int puff_index = 0; // Maintian the number of the particles in puff
int explosion_index = 0;  // Maintian the number of the particles in explosion
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
Asteroid small_asteroids[NUMBER_OF_ASTEROID * 2];
Vector2D small_asteroid_Postions[NUMBER_OF_ASTEROID * 2];
Vector2D small_asteroid_directions[NUMBER_OF_ASTEROID * 2];
//keyboard
Keyboard keyboard;
//color
Color arena_color;
Color asteroids_color;
Color puff_color;
Color* particle_color;
//bulllets
Bullet bullets[NUMBER_OF_BULLETS];
Vector2D bullets_Postion[NUMBER_OF_BULLETS];
Vector2D bullets_direction[NUMBER_OF_BULLETS];
//puff
Particle puff[NUMBER_OF_PARTICLE_IN_PUFF];
Vector2D puff_Postion[NUMBER_OF_PARTICLE_IN_PUFF];
Vector2D puff_direction[NUMBER_OF_PARTICLE_IN_PUFF];
//Explosion
Particle_v2 particles[NUMBER_OF_PARTICLE_IN_EXPLOSION];
Vector2D particles_Postion[NUMBER_OF_PARTICLE_IN_EXPLOSION];
Vector2D particles_direction[NUMBER_OF_PARTICLE_IN_EXPLOSION];
Color particles_color[NUMBER_OF_PARTICLE_IN_EXPLOSION];
//Game log
Game_Log game_log;

void on_reshape(int width, int height) {
	glViewport(0, 0, g_screen_width, g_screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, g_screen_width, 0.0, g_screen_height, 1.0, -1.0);	
}
void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Render game info
	render_game_score(&game_log);
	render_game_time(&game_log);

	render_arena(&arena);
	//Render ship only when it is active
	if (ship.active == 1) {
		render_spaceShip(window.width, window.height, &ship, SHIP_SCALE_SIZE);
	}
	else
	{
		//otherwise, render game over info
		render_end_game_info(&game_log);
	}
	//render asteroids
	for (int i = 0; i < current_number_of_asteroid; i++) {
		if (asteroids[i].active == 1) {
			render_circle(window.width, window.height, &asteroids[i], asteroids[i].bounding_circle, &asteroids_color, ASTEROID_SCALE_SIZE);
		}	
	}
	//render small asteroids
	for (int i = 0; i < current_number_of_splited_asteroid; i++) {
		if (small_asteroids[i].active == 1) {
			render_circle(window.width, window.height, &small_asteroids[i], small_asteroids[i].bounding_circle, &asteroids_color, ASTEROID_SCALE_SIZE);
		}
	}
	//render bullets
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		if (bullets[i].fired == 1) {
			render_bullet(&window, &ship, &bullets[i]);
		}
	}
	//render puff
	for (int i = 0; i < NUMBER_OF_PARTICLE_IN_PUFF; i++) {
		if (puff[i].active == 1) {
			render_particle(&window, &ship, &puff[i]);
		}
	}
	//render explosion
	for (int i = 0; i < NUMBER_OF_PARTICLE_IN_EXPLOSION; i++) {
		if (particles[i].active == 1) {
			render_explosion(&window, &particles[i], NUMBER_OF_PARTICLE_IN_EXPLOSION, SHIP_SCALE_SIZE);
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
		//Retset Game
		if (ship.active == 0) {
			reset_ship(&ship, &window, window.width * START_POS_RATIO + arena.left, window.height * START_POS_RATIO + arena.bottom);
			reset_puff(&puff, NUMBER_OF_PARTICLE_IN_PUFF);
			reset_explosion(&particles, NUMBER_OF_PARTICLE_IN_EXPLOSION);
			//reset asteroids
			for (int i = 0; i < NUMBER_OF_ASTEROID; i++) {
				launch_asteroid(&ship, &asteroids[i], &asteroid_directions[i], &asteroid_Postions[i],
					window.width, window.height, ASTEROID_VELOCITY, ASTEROID_ANGULAR_VELOCITY, ASTEROID_SCALE_SIZE);
			}
			for (int i = 0; i < NUMBER_OF_ASTEROID * 2; i++) {
				launch_asteroid(&ship, &small_asteroids[i], &small_asteroid_directions[i], &small_asteroid_Postions[i],
					window.width, window.height, ASTEROID_VELOCITY, ASTEROID_ANGULAR_VELOCITY, ASTEROID_SCALE_SIZE);
			}
			current_number_of_asteroid = 1;
			explosion_index = 0;
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
	//get time
	cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	//update game time
	update_game_time(&game_log,cur_time, minutes, seconds);
	//get the time interval between frames
	dt = cur_time - g_last_time;
	//Update the asteriod direction towards the ship
	if (asteroids[current_number_of_asteroid].active == 0) {
		if (asteroids[current_number_of_asteroid].active == 0) {
			asteroids[current_number_of_asteroid].active = 1;
			//set direction to a point that asteroid towards ship
			asteroids[current_number_of_asteroid].direction->x = ship.position->x;
			asteroids[current_number_of_asteroid].direction->y = ship.position->y;
			//normalizing from asteroid's position
			normalizing(asteroids[current_number_of_asteroid].direction, 
				asteroids[current_number_of_asteroid].position->x, 
				asteroids[current_number_of_asteroid].position->y);
		};
	}
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
	//Bouns feature 1. Better Ship Movement
	increasing_speed(&ship, &keyboard, dt, SHIP_MAX_VELOCITY, 30);
	decreasing_speed(&ship, &keyboard, dt, SHIP_MAX_VELOCITY, 30);

	//Movement of the ship
	if (keyboard.forward == 1) {
		update_ship_position(&ship,ship.current_degree, ship.velocity * dt);
		g_particle_hold += dt;
		if (g_particle_hold > 0.15) {	
			puff_release(&ship,&puff[puff_index], g_screen_width * 0.02f);
			g_particle_hold = 0;
			puff_index++;
			if (puff_index >= NUMBER_OF_PARTICLE_IN_PUFF) {
				puff_index = 0;
			}
		}		
	}
	else {
		update_ship_position(&ship, ship.current_degree, ship.velocity * dt);
	}
	//Ship rotation
	if (keyboard.anti_clockwise == 1) {
		update_ship_angle(&ship, SHIP_AV * dt);
	}
	if (keyboard.clockwise == 1) {
		update_ship_angle(&ship, -(SHIP_AV * dt));
	}

	//Movement of an astroid
	for (int i = 0; i < current_number_of_asteroid; i++) {
		update_asteroid_position(&asteroids[i], window.width, window.height, asteroids[i].velocity * dt);
		update_asteroid_angle(&asteroids[i], ASTEROID_ANGULAR_VELOCITY * dt);
		//checking collision
		spaceship_asteroid_collision(&ship, &asteroids[i]);
	}

	for (int i = 0; i < current_number_of_splited_asteroid; i++) {
		if (small_asteroids[i].active == 1) {
			update_asteroid_position(&small_asteroids[i], window.width, window.height, small_asteroids[i].velocity * dt);
			update_asteroid_angle(&small_asteroids[i], ASTEROID_ANGULAR_VELOCITY * dt);
			//checking collision
			spaceship_asteroid_collision(&ship, &small_asteroids[i]);
		}	
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

	//checking collision between asteroids and bullets
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		for (int j = 0; j < NUMBER_OF_ASTEROID; j++) {
			int is_collision = bullet_asteroid_collision(&bullets[i], &asteroids[j]);
			if (is_collision == 2) {
				//Start a explosion
				explosion_starts(&asteroids[j], particles, NUMBER_OF_PARTICLE_IN_EACH_EXPLOSION, explosion_index);
				explosion_index += NUMBER_OF_PARTICLE_IN_EACH_EXPLOSION;
				if (explosion_index  >= NUMBER_OF_PARTICLE_IN_EXPLOSION - 20) {
					explosion_index = 0;
				}
				//Update game info
				update_game_score(&game_log);

				split_asteroid(&asteroids[j], 
					&small_asteroids[current_number_of_splited_asteroid],
					&small_asteroids[current_number_of_splited_asteroid + 1],
					g_screen_height, ASTEROID_SCALE_SIZE);
				if (current_number_of_splited_asteroid < NUMBER_OF_ASTEROID * 2) {
					current_number_of_splited_asteroid += 2;
				}	
			}
		}
	}
	//checking collision between small asteroids and bullets
	for (int i = 0; i < NUMBER_OF_BULLETS; i++) {
		for (int j = 0; j < NUMBER_OF_ASTEROID*2; j++) {
			int is_collision = bullet_asteroid_collision(&bullets[i], &small_asteroids[j]);
			if (is_collision == 2) {
				//Start a explosion
				explosion_starts(&small_asteroids[j], particles, NUMBER_OF_PARTICLE_IN_EACH_EXPLOSION, explosion_index);
				explosion_index += NUMBER_OF_PARTICLE_IN_EACH_EXPLOSION;
				if (explosion_index >= NUMBER_OF_PARTICLE_IN_EXPLOSION - 20) {
					explosion_index = 0;
				}
				//Update game info
				update_game_score(&game_log);
			}
		}
	}
	//updating puff
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
	//Check collsion between asteriods and arena
	for (int i = 0; i < NUMBER_OF_ASTEROID; i++) {
		asteroid_hit_arena(&asteroids[i], &arena);
	}
	//Check collsion between small asteriods and arena
	for (int i = 0; i < NUMBER_OF_ASTEROID * 2; i++) {
		asteroid_hit_arena(&small_asteroids[i], &arena);
	}
	//check collsion between asteriods
	for (int i = 0; i < NUMBER_OF_ASTEROID - 1; i++) {
		for (int j = 1; j < NUMBER_OF_ASTEROID; j++) {
			if (i != j) {
				asteroid_hit_asteroid(&asteroids[i], &asteroids[j], &arena);
			}	
		}
	}
	//check collsion between small asteriods
	for (int i = 0; i < NUMBER_OF_ASTEROID*2 - 1; i++) {
		for (int j = 1; j < NUMBER_OF_ASTEROID*2; j++) {
			if (i != j) {
				asteroid_hit_asteroid(&small_asteroids[i], &small_asteroids[j], &arena);
			}
		}
	}
	//check collsion between small asteriods and the big one
	for (int i = 0; i < NUMBER_OF_ASTEROID ; i++) {
		for (int j = 0; j < NUMBER_OF_ASTEROID * 2; j++) {
			asteroid_hit_asteroid(&asteroids[i], &small_asteroids[j], &arena);
		}
	}

	//update explosion
	for (int i = 0; i < explosion_index; i++) {
		if (particles[i].active == 1) {
			update_position(particles[i].direction, particles[i].position, 0, particles[i].velocity* dt);
			particles[i].lifespan -= dt;
			if (particles[i].lifespan < 0) {
				particles[i].lifespan = EXPLOSION_LIFESPAN;
				particles[i].active = 0;
			}
		}
		else {
			particles[i].position->x = 0;
			particles[i].position->y = 0;
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
	//init the ship, for the bouns feature, we set the ship speed to 0 at the beginning
	ship2d_t_init(&ship, &shipDirection, &shipPostion, &ship_outline_color, &ship_filling_color,
			0, SHIP_AV, SHIP_SCALE_SIZE * g_screen_width);
	//init asteroids, both normal one and the splitted one
	for (int i = 0; i < NUMBER_OF_ASTEROID; i++) {
		launch_asteroid(&ship, &asteroids[i], &asteroid_directions[i], &asteroid_Postions[i],
			window.width, window.height, ASTEROID_VELOCITY, ASTEROID_ANGULAR_VELOCITY, ASTEROID_SCALE_SIZE);
	}
	for (int i = 0; i < NUMBER_OF_ASTEROID * 2; i++) {
		launch_asteroid(&ship, &small_asteroids[i], &small_asteroid_directions[i], &small_asteroid_Postions[i],
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

	explosion_init(&particles, &particles_Postion, &particles_direction, &particles_color, NUMBER_OF_PARTICLE_IN_EXPLOSION,
		EXPLOSION_LIFESPAN,PARTICLE_SIZE,PARTICLE_VELOCITY);

	//for (int i = 0; i < NUMBER_OF_PARTICLE_IN_EXPLOSION; i++) {
	//	printf("%d,%f,%f\n", i, particles[i].direction[i].x, particles[i].direction[i].y);
	//}
	//init keyboard stats
	keyboard_init(&keyboard);
}

void init(int* argc, char** argv)
{

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	g_screen_width = glutGet(GLUT_SCREEN_WIDTH);
	g_screen_height = glutGet(GLUT_SCREEN_HEIGHT);
	glutCreateWindow("Assignment 1 - s3795574");
	glutFullScreen();
	glutReshapeFunc(on_reshape);
	glutDisplayFunc(on_display);

	glutMouseFunc(on_mouse_button);
	glutKeyboardFunc(on_keyboard_press);
	glutKeyboardUpFunc(on_keyboard_release);

	glutIdleFunc(on_idle);

	init_game();
}

int main(int argc, char** argv)
{

	srand(time(NULL)); //seed the random generator
	init(&argc, argv);
	glutMainLoop();
	return EXIT_SUCCESS;
}

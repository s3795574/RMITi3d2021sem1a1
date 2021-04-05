#include "i3d_Game_Objects.h"
#include "i3d_Game_Math.h"
#define _USE_MATH_DEFINES
#include <math.h>


void vec2d_t_init(Vector2D* vector2d, float x, float y) {
	vector2d->x = x;
	vector2d->y = y;
}

void ship2d_t_init(Spaceship* ship2d, Vector2D* dir, Vector2D* pos, float vel, float degree) {
	ship2d->direction = dir;
	ship2d->position = pos;
	ship2d->velocity = vel;
	ship2d->av = degree;
	ship2d->current_degree = getAngleInDegree(dir);
}

void update_ship_position(Spaceship* ship2d, float degree, float movement) {
	update_position(ship2d->direction,ship2d->position,ship2d->current_degree, movement);
}

void update_ship_angle(Spaceship* ship2d, float degree) {
	ship2d->current_degree = ship2d->current_degree + degree; //update current degree
	/*
	* two if statements below will make sure that the angle is between 0 and 360
	*/
	if (ship2d->current_degree >= 360 || ship2d->current_degree <= -360) {
		ship2d->current_degree = (int)(ship2d->current_degree) % 360;
	}
	if (ship2d->current_degree < 0) {
		ship2d->current_degree = 360 + ship2d->current_degree;
	}
	update_direction(ship2d->direction, ship2d->current_degree); //update direction vector
}
void keyboard_init(Keyboard* keyboard) {
	keyboard->w = 0;
	keyboard->a = 0;
	keyboard->d = 0;
}

void color_init(Color* color) {
	color->R = 1;
	color->B = 1;
	color->G = 1;
}

void set_arena(Arena* arena, float left, float right, float top, float bottom) {
	arena->left = left;
	arena->right = right;
	arena->top = top;
	arena->bottom = bottom;
}

int hit_wall(Spaceship* ship2d, Arena* arena) {
	int ship_x = ship2d->position->x;
	int ship_y = ship2d->position->y;

	if (ship_x <= arena->left + ship2d->radius || ship_x >= arena->right - ship2d->radius || 
		ship_y >= arena->top - ship2d->radius || ship_y <= arena->bottom + ship2d->radius) {
		return 2;
	}
	if (ship_x <= arena->left + ship2d->radius*2 || ship_x >= arena->right - ship2d->radius*2 || 
		ship_y >= arena->top - ship2d->radius*2 || ship_y <= arena->bottom + ship2d->radius*2) {
		return 1;
	}
	return 0;
}

int reset_ship(Spaceship* ship2d, float x, float y) {
	ship2d->position->x = x;
	ship2d->position->y = y;
}

void launch_asteroid(Spaceship* ship2d, Asteroid* asteroid, 
					Vector2D* ast_dir, Vector2D* ast_pos, 
					int width, int height, float vel, float av) {
	
	float angle = (360 / 20) / (180.0 / M_PI);//get radian for each triangle
	float circle[3 * 20];//each vertex needs three values
	for (int i = 0; i < 20; i++) {
		float random_ratio = (rand() % 30) * 0.01;
		asteroid->vertex[i * 3] = cosf(angle * i) * (1 + random_ratio) ;//X
		asteroid->vertex[i * 3 + 1] = sinf(angle * i) * (1 + random_ratio); //Y
		asteroid->vertex[i * 3 + 2] = 0; //Z is always 0 in 2D
	}
	//init the position
	vec2d_t_init(ast_pos, width, height);
	int random_degree = rand()%360; //get a random degree
	float radian = random_degree / (180.0 / M_PI);//get radian from degree
	float radius = getLength2D(width/2, height/2);
	ast_pos->y = sinf(radian)  * radius + height/2;
	ast_pos->x = cosf(radian) * radius + width/2;
	//set direction to a point that asteroid towards
	ast_dir->x = ship2d->position->x;
	ast_dir->y = ship2d->position->y;
	//normalizing from asteroid's position
	normalizing(ast_dir, ast_pos->x, ast_pos->y);
	//get a random speed between -10% and 10% of the predefined speed
	float random_ratio = (rand() % 30) * 0.01; // a random number between 0 and 0.1
	if (random_degree % 2 == 1) {// either the result is 0 or 1
		asteroid->velocity = vel * (1 + random_ratio);
	}
	else {
		asteroid->velocity = vel * (1 - random_ratio);
	}
	
	asteroid->direction = ast_dir;
	asteroid->position = ast_pos;
	//debug info
	printf("the random degree and lauch pos and direction is : %d , %f, %f, %f, %f, %f\n",
		random_degree, ast_pos->x, ast_pos->y, ast_dir->x, ast_dir->y, asteroid->radius);
}

void update_asteroid_position(Asteroid* asteroid, int width, int height,  float movement) {
	//keep asteroid in the screen
	if (asteroid->position->x < 0) {
		asteroid->position->x = width;
	}
	if (asteroid->position->x > width) {
		asteroid->position->x = 0;
	}
	if (asteroid->position->y < 0) {
		asteroid->position->y = height;
	}
	if (asteroid->position->y > height) {
		asteroid->position->y = 0;
	}
	update_position(asteroid->direction, asteroid->position,0,movement);
}
//return 1 if they collided
int spaceship_asteroid_collision(Spaceship* ship2d, Asteroid* asteroid) {
	int distance = getDistance(ship2d->position, asteroid->position);
	if (distance <= ship2d->radius + asteroid->radius) {
		return 1;
	}
	else {
		return 0;
	}
}

void asteroids_init(Spaceship* ship2d, Asteroid asteroid[],
					Vector2D dir[], Vector2D pos[],
					int width, int height, float vel, float degree) {

}
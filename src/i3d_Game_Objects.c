#include "i3d_Game_Objects.h"
#include "i3d_Game_Math.h"


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

void update_ship_position(Spaceship* ship2d, float length) {
	update_position(ship2d->direction,ship2d->position,ship2d->current_degree,length); 
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
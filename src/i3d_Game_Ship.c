#include "i3d_Game_Ship.h"

void ship2d_t_init(Spaceship* ship2d, Vector2D* dir, Vector2D* pos, float vel, float degree) {
	ship2d->direction = dir;
	ship2d->position = pos;
	ship2d->velocity = vel;
	ship2d->av = degree;
	ship2d->current_degree = getAngleInDegree(dir);
}

void update_ship_position(Spaceship* ship2d, float degree, float movement) {
	update_position(ship2d->direction, ship2d->position, ship2d->current_degree, movement);
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
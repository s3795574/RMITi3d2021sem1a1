#include "i3d_Game_Objects.h"
#include "i3d_Game_Math.h"
#define _USE_MATH_DEFINES
#include <math.h>


void vec2d_t_init(Vector2D* vector2d, float x, float y) {
	vector2d->x = x;
	vector2d->y = y;
}

void window_init(Game_Window* window, int width, int height,int full_screen) {
	window->width = width;
	window->height = height;
	window->full_screen = full_screen;
}

void keyboard_init(Keyboard* keyboard) {
	keyboard->forward = 0;
	keyboard->anti_clockwise = 0;
	keyboard->clockwise = 0;
}

void color_init(Color* color, float r, float g, float b) {
	color->R = r;
	color->G = g;
	color->B = b;
}

void arena_init(Arena* arena, Color* color, int width, int height, float scale) {
	arena->color = color;
	arena->scale_ratio = scale;
	arena->width = width;
	arena->height = height;
	arena->left = width * (1 - scale) / 2;
	arena->right = width - arena->left;
	arena->bottom = height * (1 - scale) / 2;
	arena->top = height - arena->bottom;
}

int hit_wall(Spaceship* ship2d, Arena* arena, float warning_ratio) {
	int ship_x = ship2d->position->x;
	int ship_y = ship2d->position->y;

	if (ship_x <= arena->left + ship2d->bounding_circle || ship_x >= arena->right - ship2d->bounding_circle || 
		ship_y >= arena->top - ship2d->bounding_circle || ship_y <= arena->bottom + ship2d->bounding_circle) {
		ship2d->active = 0;
		return 2;
	}
	if (ship_x <= arena->left + ship2d->bounding_circle* warning_ratio || ship_x >= arena->right - ship2d->bounding_circle* warning_ratio ||
		ship_y >= arena->top - ship2d->bounding_circle* warning_ratio || ship_y <= arena->bottom + ship2d->bounding_circle* warning_ratio) {
		return 1;
	}
	return 0;
}

//return 1 if they collided
int spaceship_asteroid_collision(Spaceship* ship2d, Asteroid* asteroid) {
	int distance = getDistance(ship2d->position, asteroid->position);
	if (ship2d->active == 1 && asteroid->active == 1 && distance <= ship2d->bounding_circle + asteroid->bounding_circle) {
		ship2d->active = 0;
		return 1;
	}
	else {
		return 0;
	}
}
//return 1 if bullet hit the asteroid, return 2 if the asteroid destoryed
int bullet_asteroid_collision(Bullet* bullet, Asteroid* asteroid) {
	int distance = getDistance(bullet->position, asteroid->position);
	if (bullet->fired == 1 && asteroid->active == 1 && distance <= asteroid->bounding_circle) {
		asteroid->hp = asteroid->hp - 1;
		if (asteroid->hp == 2) {
			asteroid->B = 0;
			bullet->fired = 0;
		}
		else if (asteroid->hp == 1) {
			asteroid->G = 0;
			bullet->fired = 0;
		}
		else {
			asteroid->active = 0;
			bullet->fired = 0;
			return 2;
		}	
		bullet->fired = 0;
		return 1;
	}
	else {
		return 0;
	}
}

int bullet_arena_collision(Bullet* bullet, Arena* arena) {
	if (bullet->fired == 1) {
		if (bullet->position->x <= arena->left || bullet->position->x >= arena->right
			|| bullet->position->y <= arena->bottom || bullet->position->y >= arena->top) {
			bullet->fired = 0;
			return 1;
		}
	}
	return 0;
}

void ship2d_t_init(Spaceship* ship2d, Vector2D* dir, Vector2D* pos, Color* outline, Color* filling, float vel, float degree, float radius) {
	ship2d->outline = outline;
	ship2d->filling = filling;
	ship2d->direction = dir;
	ship2d->position = pos;
	ship2d->velocity = vel;
	ship2d->av = degree;
	ship2d->current_degree = getAngleInDegree(dir);
	ship2d->bounding_circle = radius;
	ship2d->active = 1;
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

int reset_ship(Spaceship* ship2d, Game_Window* window,float x, float y) {
	ship2d->position->x = x;
	ship2d->position->y = y;
	//Reset the point that ship towards
	ship2d->direction->x = window->width;
	ship2d->direction->y = window->height;
	//Normalizing the direction
	normalizing(ship2d->direction, x, y);
	//Get the angle
	ship2d->current_degree = getAngleInDegree(ship2d->direction);
	ship2d->velocity = 0;
	ship2d->active = 1;
}

void bullet_init(Bullet* bullet, Spaceship* ship, Vector2D* dir, Vector2D* pos, float velocity) {
	bullet->direction = dir;
	bullet->position = pos;
	bullet->position->x = ship->position->x;
	bullet->position->y = ship->position->y;
	bullet->direction->x = ship->direction->x;
	bullet->direction->y = ship->direction->y;
	bullet->velocity = velocity;
	bullet->fired = 0;
}

void update_bullet_position(Bullet* bullet, float movement) {
	update_position(bullet->direction, bullet->position, 0, movement);
}

void launch_asteroid(Spaceship* ship2d, Asteroid* asteroid,
	Vector2D* ast_dir, Vector2D* ast_pos,
	int width, int height, float vel, float av, float scale_size) {

	float angle = (360 / 20) / (180.0 / M_PI);//get radian for each triangle
	//float circle[3 * 20];//each vertex needs three values
	for (int i = 0; i < 20; i++) {
		float unique_ratio = random_raito(10);
		asteroid->vertex[i * 3] = cosf(angle * i) * (1 + unique_ratio);//X
		asteroid->vertex[i * 3 + 1] = sinf(angle * i) * (1 + unique_ratio); //Y
		asteroid->vertex[i * 3 + 2] = 0; //Z is always 0 in 2D
	}
	//init the position
	vec2d_t_init(ast_pos, width, height);
	int random_degree = rand() % 360; //get a random degree
	float radian = random_degree / (180.0 / M_PI);//get radian from degree
	float random_ratio = (rand() % 20) * 0.01; // a random number between 0 and 0.1
	//Set values in a random range 
	if (random_degree % 2 == 1) {// either the result is 0 or 1
		asteroid->velocity = vel * (1 + random_ratio);
		asteroid->clockwise = 1;
		//a slightly larger radius(bouding circle) will have 3 hit point.
		asteroid->bounding_circle = scale_size * width * (1 + random_ratio);
		asteroid->radius = (1 + random_ratio);
		//larger asteroid has 3 hp
		asteroid->hp = 3;
	}
	else {
		asteroid->velocity = vel * (1 - random_ratio);
		asteroid->clockwise = 0;
		//a slightly smaller radius(bouding circle) will have 2 hit point.
		asteroid->bounding_circle = scale_size * width * (1 - random_ratio);
		asteroid->radius = (1 - random_ratio);
		//smaller asteroid has 2 hp
		asteroid->hp = 2;
	}
	//The launch position is on a circle with radius =  launch_pos, the center is the screen center
	float launch_pos = getLength2D(width / 2, height / 2); 
	ast_pos->y = sinf(radian) * launch_pos + height / 2;
	ast_pos->x = cosf(radian) * launch_pos + width / 2;
	//The code below has no use, the direction will be set when the asteroid become active.
	//ast_dir->x = ship2d->position->x;
	//ast_dir->y = ship2d->position->y;
	////normalizing from asteroid's position
	//normalizing(ast_dir, ast_pos->x, ast_pos->y);

	asteroid->direction = ast_dir;
	asteroid->position = ast_pos;
	asteroid->active = 0;
	asteroid->R = 0.5;
	asteroid->G = 0.5;
	asteroid->B = 0.5;
}

void split_asteroid(Asteroid* asteroid, Asteroid* asteroid_left, Asteroid* asteroid_right,int width, float scale_size) {
	//calculate the new position of splitted asteroids.
	float deltaX, deltaY;
	//Create three separate direction for asteroid, left asteroid and right asteroid
	Vector2D dir, left_dir, right_dir;
	dir.x = asteroid->direction->x;
	dir.y = asteroid->direction->y;
	left_dir.x = asteroid->direction->x;
	left_dir.y = asteroid->direction->y;
	right_dir.x = asteroid->direction->x;
	right_dir.y = asteroid->direction->y;
	float asteroid_degree = getAngleInDegree(&dir);
	printf("Before:%f,%f\n", asteroid->direction->x, asteroid->direction->y);
	//rotation to the left
	float clockwise_45_degree = asteroid_degree + 45;
	update_direction(asteroid_left->direction, clockwise_45_degree);
	float degree_after_clockwise_45_degree = getAngleInDegree(asteroid_left->direction);
	float counter_clockwise_45_degree_45_degree = asteroid_degree - 45;
	update_direction(asteroid_right->direction, counter_clockwise_45_degree_45_degree);
	float degree_after_counter_clockwise_45_degree = getAngleInDegree(asteroid_right->direction);
	//Calculate the new position for the left
	//rotation(asteroid->direction, -90);
	update_direction(&left_dir, asteroid_degree + 90);
	deltaX = left_dir.x * width * scale_size;
	deltaY = left_dir.y * width * scale_size;
	asteroid_left->position->x = asteroid->position->x + deltaX;
	asteroid_left->position->y = asteroid->position->y + deltaY;
	asteroid_left->bounding_circle = asteroid->bounding_circle / 2 * 0.9;
	asteroid_left->radius = asteroid->radius / 2 * 0.9;
	asteroid_left->av = asteroid->av;
	asteroid_left->clockwise = 1;
	asteroid_left->velocity = asteroid->velocity;
	asteroid_left->hp = 1;
	asteroid_left->active = 1;

	//reverse, it equals to ratation to the right for 90 degree
	//rotation(asteroid->direction, 180);
	update_direction(&right_dir, asteroid_degree - 90);
	deltaX = right_dir.x * width * scale_size;
	deltaY = right_dir.y * width * scale_size;
	asteroid_right->position->x = asteroid->position->x + deltaX;
	asteroid_right->position->y = asteroid->position->y + deltaY;
	asteroid_right->bounding_circle = asteroid->bounding_circle / 2 * 0.9;
	asteroid_right->radius = asteroid->radius / 2 * 0.9;
	asteroid_right->av = asteroid->av;
	asteroid_right->clockwise = 0;
	asteroid_right->velocity = asteroid->velocity;
	asteroid_right->hp = 1;
	asteroid_right->active = 1;
	printf("After:%f,%f\n", asteroid_left->direction->x, asteroid_left->direction->y);
	printf("After:%f,%f\n", asteroid_right->direction->x, asteroid_right->direction->y);
	printf("After:%f,%f\n", asteroid_left->position->x, asteroid_left->position->y);
	printf("After:%f,%f\n", asteroid_right->position->x, asteroid_right->position->y);
}

void update_asteroid_position(Asteroid* asteroid, int width, int height, float movement) {
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
	update_position(asteroid->direction, asteroid->position, 0, movement);
}

void update_asteroid_angle(Asteroid* asteroid, float degree) {
	if (asteroid->clockwise == 0) {
		asteroid->current_degree = asteroid->current_degree + degree;
	}
	if (asteroid->clockwise == 1) {
		asteroid->current_degree = asteroid->current_degree - degree;
	}


	/*
	* two if statements below will make sure that the angle is between 0 and 360
	*/
	if (asteroid->current_degree >= 360 || asteroid->current_degree <= -360) {
		asteroid->current_degree = (int)(asteroid->current_degree) % 360;
	}
	if (asteroid->current_degree < 0) {
		asteroid->current_degree = 360 + asteroid->current_degree;
	}
}
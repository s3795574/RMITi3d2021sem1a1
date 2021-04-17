#include "i3d_Game_Math.h"
#define _USE_MATH_DEFINES
#include <math.h>


float random_raito(int percentage) {
	float ratio = (rand() % percentage) * 0.01;
	if (rand() % 2 == 1) {
		ratio = ratio * -1;
	}
	return ratio;
}

float getLength2D(float x, float y) {
	return sqrtf(x * x + y * y);
}

float getDistance(Vector2D* point1, Vector2D* point2) {
	return sqrtf((point2->x - point1->x) * (point2->x - point1->x) + (point2->y - point1->y) * (point2->y - point1->y));
}
void normalizing(Vector2D* direction, float originX, float originY) {
	direction->length = getLength2D((direction->x - originX), (direction->y - originY));
	direction->x = (direction->x - originX) / direction->length;
	direction->y = (direction->y - originY) / direction->length;
	direction->length = 1;
}

float getAngleInDegree(Vector2D* direction) {
	return atan2f(direction->y,direction->x) * (180.0 / M_PI);
}

void update_position(Vector2D* direction, Vector2D* position,float degree, float movement) {
	float radian = degree /(180.0 / M_PI);
	float deltaY = direction->y * movement;
	float deltaX = direction->x * movement;
	position->x = position->x + deltaX;
	position->y = position->y + deltaY;
}

void update_direction(Vector2D* direction, float degree) {
	float radian = degree / (180.0 / M_PI);
	direction->x = cosf(radian);
	direction->y = sinf(radian) ;
}

float get_normal_x(float x1, float x2, float distance) {
	float nx = (x1 - x2) / distance;
	return nx;
}
float get_normal_y(float y1, float y2, float distance) {
	float ny = (y1 - y2) / distance;
	return ny;
}

float get_dot_product(float x1, float y1, float x2, float y2) {
	return x1 * x2 + y1 * y2;
}
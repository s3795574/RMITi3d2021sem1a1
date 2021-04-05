#include "i3d_Game_Math.h"
#define _USE_MATH_DEFINES
#include <math.h>



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
	//float deltaY = sinf(radian) * movement;
	//float deltaX = cosf(radian) * movement;
	float deltaY = direction->y * movement;
	float deltaX = direction->x * movement;
	position->x = position->x + deltaX;
	position->y = position->y + deltaY;
}

void update_direction(Vector2D* direction, float degree) {
	float radian = degree / (180.0 / M_PI);
	direction->x = cosf(radian);
	//if (degree > 90 && degree < 270) {
	//	direction->y = sinf(radian) * -1;
	//}
	direction->y = sinf(radian) ;
}
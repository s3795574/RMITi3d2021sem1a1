#include "i3d_Game_Math.h"
#define _USE_MATH_DEFINES
#include <math.h>



float getLength2D(float x, float y) {
	return sqrtf(x * x + y * y);
}

void normalizing(Vector2D* direction) {
	direction->length = getLength2D(direction->x, direction->y);
	direction->x = direction->x / direction->length;
	direction->y = direction->y / direction->length;
	direction->length = 1;
}

float getAngleInDegree(Vector2D* direction) {
	return asinf(direction->y) * (180.0 / M_PI);
}

void update_position(Vector2D* direction, Vector2D* position,float degree, float length) {
	float radian = degree /(180.0 / M_PI);
	float deltaY = sinf(radian) * length;
	float deltaX = cosf(radian) * length;
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
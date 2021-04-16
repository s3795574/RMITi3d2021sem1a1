#include "i3d_Game_Particle.h"
#define _USE_MATH_DEFINES
#include <math.h>

void puff_init(Spaceship* ship, Particle* particle, Vector2D* position, Game_Window* window,  Color* color, float radius, float lifespan, float size) {
	particle->position = position;
	particle->position->x = ship->position->x + ship->direction->x * -1.0f * window->width * 0.02f;
	particle->position->y = ship->position->y + ship->direction->y * -1.0f * window->width * 0.02f;
	float angle = (360 / 20) / (180.0 / M_PI);//get radian for each triangle
	for (int i = 0; i < 20; i++) {
		particle->vertex[i * 3] = cosf(angle * i) ;//X
		particle->vertex[i * 3 + 1] = sinf(angle * i); //Y
		particle->vertex[i * 3 + 2] = 0; //Z is always 0 in 2D
	}
	particle->color = color;
	particle->radius = radius;
	particle->lifespan = lifespan;
	particle->size = size;
	particle->active = 0;
}

void particle_release(Spaceship* ship, Particle* particle, float scale) {
	particle->active = 1;
	particle->position->x = ship->position->x + ship->direction->x * -1.0f * scale;
	particle->position->y = ship->position->y + ship->direction->y * -1.0f * scale;
}
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

void puff_release(Spaceship* ship, Particle* particle, float scale) {
	particle->active = 1;
	particle->position->x = ship->position->x + ship->direction->x * -1.0f * scale;
	particle->position->y = ship->position->y + ship->direction->y * -1.0f * scale;
}

void explosion_init(Particle_v2* particle, Vector2D* position, Vector2D* direction, Color* color, int array_size, float lifespan, float size,float velocity) {
	for (int i = 0; i < array_size; i++) {
		int random_degree = rand() % 360; //get a random degree
		float radian = random_degree / (180.0 / M_PI);//get radian from degree
		direction[i].y = sinf(radian);
		direction[i].x = cosf(radian);

		position[i].x = 0;
		position[i].y = 0;

		color[i].R = rand() % 10 * 0.1;
		color[i].G = rand() % 10 * 0.1;
		color[i].B = rand() % 10 * 0.1;
	
		particle[i].position = position;
		particle[i].direction = direction;
		particle[i].color = color;
		particle[i].lifespan = lifespan;
		particle[i].size = rand() % 50 * 0.01 * size;
		particle[i].active = 0;
		particle[i].velocity =  rand() % 50 * 0.01 * velocity;
	}
}

void explosion_starts(Asteroid* asteroid, Particle_v2* particles, int array_size, int number_of_explosion) {
	for (int i = number_of_explosion; i < number_of_explosion + 20; i++) {
		particles[i].position[i].x = asteroid->position->x;
		particles[i].position[i].y = asteroid->position->y;
		particles[i].active = 1;
		printf("Dir: %d,%f,%f\n", i, particles[i].direction[i].x, particles[i].direction[i].y);
		printf("Pos: %d,%f,%f\n", i, particles[i].position[i].x, particles[i].position[i].y);
	}
}

void explosion_update(Particle_v2* particle,int array_size, float dt) {

}
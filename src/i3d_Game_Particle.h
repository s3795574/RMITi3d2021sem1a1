#pragma once
#include "i3d_Game_Objects.h"

typedef struct particle{
	Vector2D* position; //current loaction of the ship
	float vertex[60]; //20 vertexs of the circle, each vertex has 3 value, x, y and z.
	float radius; // 
	float lifespan;
	float size;
	Color* color;
	int active;
}Particle;

typedef struct particle_v2 {
	Vector2D* position;
	Vector2D* direction;
	float vertex[60]; //20 particles of the explosion
	Color* color;
	float lifespan;
	float size;
	float velocity;
	int active;
}Particle_v2;

// initialize puff particle
extern void puff_init(Spaceship* ship, Particle* particle, Vector2D* position, Game_Window* window, Color* color, float radius, float lifespan, float size);
// release it from the rear of the ship
extern void puff_release(Spaceship* ship, Particle* particle, float scale);

extern void explosion_init(Particle_v2* particle, Vector2D* position, Vector2D* direction, Color* color, int array_size, float lifespan, float size, float velocity);

extern void explosion_starts(Asteroid* asteroid, Particle_v2* particle, int array_size, int number_of_explosion);

extern void explosion_update(Particle_v2* particle, int array_size, float dt);
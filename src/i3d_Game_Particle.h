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
// initialize puff particle
extern void puff_init(Spaceship* ship, Particle* particle, Vector2D* position, Game_Window* window, Color* color, float radius, float lifespan, float size);
// release it from the rear of the ship
extern void particle_release(Spaceship* ship, Particle* particle, float scale);
#pragma once
#include "i3d_Game_Bounce.h"
#include "i3d_Game_Math.h"

int asteroid_hit_arena(Asteroid* asteroid, Arena* arena) {
	if (asteroid->active == 1 && abs((int)asteroid->position->x - (int)arena->left) < asteroid->bounding_circle && asteroid->position->x > arena->left) {
		//Checking the direction, make sure the collision only happens when the asteroid is inside arena.
		if (asteroid->direction->x < 0) {
			asteroid->direction->x  = asteroid->direction->x * -1;
			return 1;
		}
	}

	if (asteroid->active == 1 && abs((int)asteroid->position->x - (int)arena->right) < asteroid->bounding_circle && asteroid->position->x < arena->right) {
		if (asteroid->direction->x > 0) {
			asteroid->direction->x = asteroid->direction->x  * -1;
			return 2;
		}

	}

	if (asteroid->active == 1 && abs((int)asteroid->position->y - (int)arena->bottom) < asteroid->bounding_circle && asteroid->position->y > arena->bottom) {
		if (asteroid->direction->y < 0) {
			asteroid->direction->y = asteroid->direction->y  * -1;
			return 3;
		}	
	}

	if (asteroid->active == 1 && abs((int)asteroid->position->y - (int)arena->top) < asteroid->bounding_circle && asteroid->position->y < arena->top) {
		if (asteroid->direction->y > 0) {
			asteroid->direction->y = asteroid->direction->y * -1;
			return 4;
		}

	}
	return 0;
}

void asteroid_hit_asteroid(Asteroid* asteroid_1, Asteroid* asteroid_2, Arena* arena) {
	//Make sure that collision check only happens inside arena
	if (asteroid_1->active == 1 && asteroid_2->active == 1 &&
		asteroid_1->position->x > arena->left && asteroid_1->position->x < arena->right
		&& asteroid_2->position->x > arena->left && asteroid_2->position->x < arena->right
		&& asteroid_1->position->y > arena->bottom && asteroid_1->position->y < arena->top
		&& asteroid_2->position->y > arena->bottom && asteroid_2->position->y < arena->top) {
		float distance = getDistance(asteroid_1->position, asteroid_2->position);
		//collision happens
		if (distance <= asteroid_1->bounding_circle + asteroid_2->bounding_circle) {
			//float nx = (asteroid_1->position->x - asteroid_2->position->x) / distance;
			//float ny = (asteroid_1->position->y - asteroid_2->position->y) / distance;
			float nx = get_normal_x(asteroid_1->position->x , asteroid_2->position->x, distance);
			float ny = get_normal_y(asteroid_1->position->y , asteroid_2->position->y, distance);
			float reverse_direction_x_asteroid_1 = asteroid_1->direction->x * -1;
			float reverse_direction_y_asteroid_1 = asteroid_1->direction->y * -1;
			float dot_product = get_dot_product(reverse_direction_x_asteroid_1, reverse_direction_y_asteroid_1,nx, ny);
			float i_proj_x = dot_product * nx;
			float i_proj_y = dot_product * ny;
			float i_perp_x = reverse_direction_x_asteroid_1 - i_proj_x;
			float i_perp_y = reverse_direction_y_asteroid_1 - i_proj_y;
			float reflection_x = reverse_direction_x_asteroid_1 - 2 * i_perp_x;
			float reflection_y = reverse_direction_y_asteroid_1 - 2 * i_perp_y;
			asteroid_1->direction->x = reflection_x;
			asteroid_1->direction->y = reflection_y;
		}
	}
}
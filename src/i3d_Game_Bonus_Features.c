#include "i3d_Game_Objects.h"

float increasing_speed(Spaceship* ship, Keyboard* keyboard, float dt, float max_speed, float acceleration) {
	if (keyboard->forward == 1 && ship->velocity < max_speed) {
		ship->velocity += acceleration * dt;
	}
}

float decreasing_speed(Spaceship* ship, Keyboard* keyboard, float dt, float max_speed, float acceleration) {
	if (keyboard->forward == 0 && ship->velocity > 0) {
  		ship->velocity -= acceleration * dt;
		//Make sure the velocity is not negetive, otherwise the ship will backward once pressed the key
		if (ship->velocity < 0) {
			ship->velocity = 0;
		}
	}
}
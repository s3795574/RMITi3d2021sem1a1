#pragma once

//Arena configuration
#define ARENA_SCALE_SIZE 0.9 //the ratio of the width and height of the screen
//default color is white
#define ARENA_COLOR_R 1
#define ARENA_COLOR_G 1
#define ARENA_COLOR_B 1
#define WARNING_RAITO 2 //warning distance is the warning_ratio * ship radius, set it lesn than 1 is useless
//Ship configuration
#define START_POS_RATIO 0.1 // How far the ship is away from the bottom left of the arena. 0.1 is 10%
#define SHIP_MAX_VELOCITY 160 // speed in pixel/s
#define SHIP_ACCELERATION 30 // How fast the ship could achieve the max speed, this is a linear function.
#define SHIP_DECELERATION 40 // This parameter controls the distance that the ship taxiing after release the forward key.
#define SHIP_AV 90 // angular velocity, degree per second
#define SHIP_SCALE_SIZE 0.02 // 
//ship outline color
#define SHIP_OUTLINE_R 1
#define SHIP_OUTLINE_G 1
#define SHIP_OUTLINE_B 1
//ship filling color
#define SHIP_FILLING_R 0
#define SHIP_FILLING_G 0
#define SHIP_FILLING_B 1
//Key configuration. Only ASCII Code
#define KEY_ESC 27
#define KEY_FORWARD 119
#define KEY_ANIT_CLOCKWISE 97
#define KEY_CLOCKWISE 100
//Asteroid configuration
#define ASTEROID_SCALE_SIZE 0.02//
#define ASTEROID_VELOCITY 80 // speed in pixel/s, the velocity will be random in the range of +/- 10% of this value
#define ASTEROID_ANGULAR_VELOCITY 20 // how fast the asterioid rotating
#define NUMBER_OF_ASTEROID 10 // there will be four waves. each wave will release 1,2,3 and 4 asteroid respectively.
#define WAVE_INTERVAL 5 // the time between each wave of asteroids
#define ASTEROID_COLOR_R 0
#define ASTEROID_COLOR_G 1
#define ASTEROID_COLOR_B 0
//Decay of puff
#define PARTICLE_DECAY 1.0f // the higher this value, the fast puff disappears
//Bullet configuration
#define BULLET_VELOCITY 200 // speed in pixel/s, set it large to make the game eaiser
#define NUMBER_OF_BULLETS 10 // how many bullets you could have at the same time. Large number means you can keep shooting.


/**
* NOTE: The parameters below is not configurable. Changing those will impact the game mechanism.
*/
//particle parameters
#define NUMBER_OF_PARTICLE_IN_EXPLOSION 50
#define NUMBER_OF_PARTICLE_IN_PUFF 110
#define PARTICLE_SIZE 5
#define PARTICLE_VELOCITY 80
#define PARTICLE_LIFESPAN 10.f
#define MAX_PARTICLES 20;
//game environment
#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 300
#define FULL_SCREEN 0 // 0 is false, 1 is true
#define KEY_PRESSED 1
#define KEY_RELEASED 0


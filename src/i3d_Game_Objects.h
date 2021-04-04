#pragma once
//This struct respresents vector
typedef struct vec2d_t {
	float x, y; //the x, y and z represent value on each axis in coordinate, for 2d graphic, the value of z would be a constant
	float length; //the length of the vector from the origin
}Vector2D;

//This struct respresents a space ship
typedef struct ship2d_t {
	Vector2D* position; //current loaction of the ship
	Vector2D* direction; //normalize to unit vector respresenting direction
	float velocity; // ship speed in pixel per second
	float av; // angular velocity, degree per second
	float current_degree;// postive x is 0 degree
	float radius; // using this to check 
}Spaceship;

typedef struct asteroid {
	Vector2D* position; //current loaction of the ship
	Vector2D* direction; //normalize to unit vector respresenting direction
	float velocity; // ship speed in pixel per second
	float av; // angular velocity, degree per second
	float current_degree;// postive x is 0 degree
	float radius; // using this to check 
}Asteroid;

//This strcut respresents user operations on keyboard
typedef struct keyboard {
	int w;
	int a;
	int d;
}Keyboard;

typedef struct color {
	float R;
	float G;
	float B;
}Color;
//each of the variables represents a boundary of arena
typedef struct arena {
	float left;
	float right;
	float top;
	float bottom;
}Arena;

//Set the x and y in a vector. NOTE: the length is yet set, please read i3d_Game_Math.h for more.
extern void vec2d_t_init(Vector2D* vector2d, float x, float y);
//Set direction, position and velocity of the ship.
extern void ship2d_t_init(Spaceship* ship2d, Vector2D* dir, Vector2D* pos, float vel, float degree);
//initialize keybarod stats, 0 represent the key was released
extern void keyboard_init(Keyboard* keyboard);
//initialize the color to white
extern void color_init(Color* color);
//initialize arena
extern void set_arena(Arena* arena, float left, float right, float top, float bottom);
//Set a new position for ship
extern void update_ship_position(Spaceship* ship2d, float length);
//Set a new angle for ship
extern void update_ship_angle(Spaceship* ship2d, float degree);
//checking arena and the ship. 0 is safe, 1 is warning, 2 is crash.
extern int hit_wall(Spaceship* ship2d, Arena* arena);
//
extern int reset_ship(Spaceship* ship2d, float x, float y);
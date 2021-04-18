#pragma once
//This struct respresents vector
typedef struct vec2d_t {
	float x, y; //the x, y and z represent value on each axis in coordinate, for 2d graphic, the value of z would be a constant
	float length; //the length of the vector from the origin
}Vector2D;
//This struct respresents
typedef struct color {
	float R;
	float G;
	float B;
}Color;
//This struct respresents a space ship
typedef struct ship2d_t {
	Vector2D* position; //current loaction of the ship
	Vector2D* direction; //normalize to unit vector respresenting direction
	Color* outline;
	Color* filling;
	float velocity; // ship speed in pixel per second
	float av; // angular velocity, degree per second
	float current_degree;// postive x is 0 degree
	float bounding_circle; // using this to check collision
	int active;
}Spaceship;
//This struct respresents a asteroid
typedef struct asteroid {
	Vector2D* position; //current loaction of the ship
	Vector2D* direction; //normalize to unit vector respresenting direction
	float vertex[60]; //20 vertexs of the circle, each vertex has 3 value, x, y and z.
	float velocity; // ship speed in pixel per second
	float av; // angular velocity, degree per second
	float current_degree;// postive x is 0 degree
	float bounding_circle; // using this to check 
	float radius;
	int clockwise; // 0 is clockwise, 1 is counter-clockwise
	int active;
	// Hit point
	int hp;
	float R;
	float G;
	float B;
}Asteroid;
//This struct respresents a bullet
typedef struct bullet {
	Vector2D* position;
	Vector2D* direction;
	float velocity;
	int fired;
	float R;
	float G;
	float B;
}Bullet;
//This strcut respresents user operations on keyboard
typedef struct keyboard {
	int forward;
	int anti_clockwise;
	int clockwise;
}Keyboard;

//each of the variables represents a boundary of arena
typedef struct arena {
	float left;
	float right;
	float top;
	float bottom;
	int width;
	int height;
	Color* color;
	float scale_ratio;
}Arena;

typedef struct window_t {
	int pos_x;
	int pos_y;
	int width;
	int height;
	int full_screen; //0 or 1
}Game_Window;
//Set the x and y in a vector. NOTE: the length is yet set, please read i3d_Game_Math.h for more.
extern void vec2d_t_init(Vector2D* vector2d, float x, float y);
//Set direction, position and velocity of the ship.
extern void ship2d_t_init(Spaceship* ship2d, Vector2D* dir, Vector2D* pos, Color* outline, Color* filling,  float vel, float degree, float bounding_circle);
//Set a new position for ship
extern void update_ship_position(Spaceship* ship2d, float degree, float movement);
//Set a new angle for ship
extern void update_ship_angle(Spaceship* ship2d, float degree);
//reset the ship position once the ship hit the wall
extern int reset_ship(Spaceship* ship2d, Game_Window* window, float x, float y);
//initialize bullet
extern void bullet_init(Bullet* bullet, Spaceship* ship, Vector2D* dir, Vector2D* pos, float velocity, float r, float b, float g);

extern void update_bullet_position(Bullet* bullet, float movement);

extern void launch_asteroid(Spaceship* ship2d, Asteroid* asteroid, Vector2D* dir, Vector2D* pos, 
	int width, int height, float vel, float av, float scale_size, float r, float g, float b);

extern void split_asteroid(Asteroid* asteroid , Asteroid* asteroid_left, Asteroid* asteroid_right, int width, float scale_size);

extern void update_asteroid_position(Asteroid* asteroid, int width, int height, float movement);

extern void update_asteroid_angle(Asteroid* asteroid, float degree);
//initialize keybarod stats, 0 represent the key was released
extern void keyboard_init(Keyboard* keyboard);
//initialize the color to white
extern void color_init(Color* color, float r, float g, float b);
//initialize arena
extern void arena_init(Arena* arena, Color* color, int width, int height, float scale);
//initialize game windows
extern void window_init(Game_Window* window, int width, int height, int full_screen);
//checking arena and the ship. 0 is safe, 1 is warning, 2 is crash. warning distance is the warning_ratio * ship radius 
extern int hit_wall(Spaceship* ship2d, Arena* arena, float warning_ratio);
//checking collision between ship and asteroids
extern int spaceship_asteroid_collision(Spaceship* ship2d, Asteroid* asteroid);
//checking collision between bullets and asteroids
extern int bullet_asteroid_collision(Bullet* bullet, Asteroid* asteroid);

extern int bullet_arena_collision(Bullet* bullet, Arena* arena);


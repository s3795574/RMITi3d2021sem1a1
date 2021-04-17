#pragma once
#include "i3d_Game_Objects.h"

//return 1 is left, 2 is right, 3 is bottom and 4 is top.
//For this task, we only reverse the x or y direction when asteroid hit the wall. No calcualtion is needed.
extern int asteroid_hit_arena(Asteroid* asteroid, Arena* arena);
//For this task, it involves calcualting vectors 
extern void asteroid_hit_asteroid(Asteroid* asteroid_1, Asteroid* asteroid_2, Arena* arena);
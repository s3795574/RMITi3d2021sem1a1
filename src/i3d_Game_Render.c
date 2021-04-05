#include "i3d_Game_Render.h"
#define _USE_MATH_DEFINES
#include <math.h>


#define FIX_DEGREE 90 //make the 0 degree from positive X axis
// The circle will look rounder if you increase the number, however, it is a trade-off for prefermance
#define NUMBER_OF_TRIANGLES 20 

extern void render_spaceShip(int width, int height, Spaceship* ship, float ship_scale_size) {
    //float angle = getAngleInDegree(ship->direction);
    float x = ship->position->x;
    float y = ship->position->y;
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(x , y, 0.0);
    glRotatef(ship->current_degree - FIX_DEGREE, 0, 0, 1); //make the 0 degree from positive X axis
    glScalef(width * ship_scale_size, height * ship_scale_size, 0.0);
    //Draw the outline of the ship
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, -0.5, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.5, -1, 0);
    glVertex3f(0, -0.5, 0);
    glVertex3f(0.5, -1, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, -0.5, 0);
    glEnd();
    //filling color of the left part of the ship
    glColor3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, -0.5, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.5, -1, 0);
    glEnd();
    //filling color of the right part of the ship
    glColor3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.5, -1, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, -0.5, 0);
    glEnd();

    glPopMatrix();
};

extern void render_arena(int width, int height, Color* color, float arena_scale_size) {
    glPushMatrix();
    glColor3f(color->R, color->G, color->B);
    glTranslatef(width / 2 , height / 2, 0.0);
    //The size of the arena is always 90% of the screen size
    glScalef(width * arena_scale_size, height * arena_scale_size, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1, -1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(-1, 1, 0);
    glEnd();
    glPopMatrix();
};

void render_circle(int width, int height, Asteroid* asteroid ,GLfloat radius, Color* color, float asteriod_scale_size) {
    float radian = (360/ NUMBER_OF_TRIANGLES) / (180.0 / M_PI);//get radian for each triangle
    float circle[3 * NUMBER_OF_TRIANGLES];//each vertex needs three values
    for (int i = 0; i < 20; i++) {
        circle[i * 3] = cosf(radian * i);//X
        circle[i * 3 + 1] = sinf(radian * i); //Y
        circle[i * 3 + 2] = 0; //Z is always 0 in 2D
    }
    glPushMatrix();
    glColor3f(color->R, color->G, color->B);
    glTranslatef(asteroid->position->x, asteroid->position->y, 0.0);
    glScalef(width * asteriod_scale_size, height * asteriod_scale_size, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 20; i++) {
        glVertex3f(asteroid->vertex[i*3], asteroid->vertex[i * 3+1], asteroid->vertex[i * 3+2]);
    }
    glVertex3f(1,0,0); //the last vertex is the start point since we are using GL_LINE_LOOP
    glEnd();
    glPopMatrix();
}
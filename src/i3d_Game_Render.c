#include "i3d_Game_Render.h"
#define _USE_MATH_DEFINES
#include <math.h>


#define FIX_DEGREE 90 //make the 0 degree from positive X axis
#define SHIP_SCALE_SIZE 0.05 //
#define ARENA_SCALE_SIZE 0.45 //
#define ASTEROID_SCALE_SIZE 0.05 //
// The circle will look rounder if you increase the number, however, it is a trade-off for prefermance
#define NUMBER_OF_TRIANGLES 20 

extern void render_spaceShip(int width, int height, Spaceship* ship) {
    //float angle = getAngleInDegree(ship->direction);
    float x = ship->position->x;
    float y = ship->position->y;
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(x , y, 0.0);
    glRotatef(ship->current_degree - FIX_DEGREE, 0, 0, 1); //make the 0 degree from positive X axis
    glScalef(width * SHIP_SCALE_SIZE, height * SHIP_SCALE_SIZE, 0.0);
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

extern void render_arena(int width, int height, Color* color) {
    glPushMatrix();
    glColor3f(color->R, color->G, color->B);
    glTranslatef(width / 2 , height / 2, 0.0);
    //The size of the arena is always 90% of the screen size
    glScalef(width * ARENA_SCALE_SIZE, height * ARENA_SCALE_SIZE, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1, -1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(-1, 1, 0);
    glEnd();
    glPopMatrix();
};

void render_circle(int width, int height, GLfloat radius, Color* color) {
    float radian = (360/ NUMBER_OF_TRIANGLES) / (180.0 / M_PI);//get radian for each triangle
    float circle[3 * NUMBER_OF_TRIANGLES];//each vertex needs three values
    for (int i = 0; i < 20; i++) {
        circle[i * 3] = cosf(radian * i) * radius;//X
        circle[i * 3 + 1] = sinf(radian * i) * radius; //Y
        circle[i * 3 + 2] = 0; //Z is always 0 in 2D
    }
    /*for (int i = 0; i < 10; i++) {
        printf("%d,%f,%f,%f\n",i + 1,circle[i * 3],  circle[i * 3 + 1], circle[i * 3 + 2]);
    }*/
    glPushMatrix();
    glColor3f(color->R, color->G, color->B);
    glTranslatef(width / 2, height / 2, 0.0);
    glScalef(width * ASTEROID_SCALE_SIZE, height * ASTEROID_SCALE_SIZE, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 20; i++) {
        glVertex3f(circle[i*3], circle[i * 3+1], circle[i * 3+2]);
    }
    glVertex3f(1,0,0); //the last vertex is the start point since we are using GL_LINE_LOOP
    glEnd();
    glPopMatrix();
}
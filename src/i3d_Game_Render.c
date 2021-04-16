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
    glColor3f(ship->outline->R, ship->outline->G, ship->outline->B);
    glTranslatef(x , y, 0.0);
    glRotatef(ship->current_degree - FIX_DEGREE, 0, 0, 1); //make the 0 degree from positive X axis
    glScalef(width * ship_scale_size, width * ship_scale_size, 0.0);
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
    glColor3f(ship->filling->R, ship->filling->G, ship->filling->B);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, -0.5, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(-0.5, -1, 0);
    glEnd();
    //filling color of the right part of the ship
    glColor3f(ship->filling->R, ship->filling->G, ship->filling->B);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.5, -1, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, -0.5, 0);
    glEnd();

    glPopMatrix();
};

extern void render_arena(Arena* arena) {
    glPushMatrix();
    glColor3f(arena->color->R, arena->color->G, arena->color->B);
    glTranslatef(arena->width / 2 , arena->height / 2, 0.0);
    //The size of the arena is always 90% of the screen size
    glScalef(arena->width * arena->scale_ratio / 2, arena->height * arena->scale_ratio / 2, 0.0);
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
    //float circle[3 * NUMBER_OF_TRIANGLES];//each vertex needs three values
    //for (int i = 0; i < 20; i++) {
    //    circle[i * 3] = cosf(radian * i);//X
    //    circle[i * 3 + 1] = sinf(radian * i); //Y
    //    circle[i * 3 + 2] = 0; //Z is always 0 in 2D
    //}
    glPushMatrix();
    glColor3f(asteroid->R, asteroid->G, asteroid->B);
    glTranslatef(asteroid->position->x, asteroid->position->y, 0.0);
    glRotatef(asteroid->current_degree - FIX_DEGREE, 0, 0, 1);
    glScalef(width * asteriod_scale_size, width * asteriod_scale_size, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        glVertex3f(asteroid->vertex[i*3], asteroid->vertex[i * 3+1], asteroid->vertex[i * 3+2]);
    }
    glVertex3f(1,0,0); //the last vertex is the start point since we are using GL_LINE_LOOP
    glEnd();
    glPopMatrix();
}

void render_bullet(Game_Window* window, Spaceship* ship, Bullet* bullet) {
    glPushMatrix();
    glTranslatef(bullet->position->x, bullet->position->y, 0.0);
    //0.02 is the scale size of the ship, changing it will make the bullet shoot in front of the ship instead of from the ship
    glScalef(window->width * 0.02, window->width * 0.02, 0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(bullet->direction->x, bullet->direction->y, 0);
    glEnd();
    glPopMatrix();
}

void render_particle(Game_Window* window, Spaceship* ship, Particle* particle) {
    glPushMatrix();
    glColor3f(particle->color->R, particle->color->B, particle->color->G);
    glTranslatef(particle->position->x, particle->position->y, 0.0);
    glScalef(particle->radius, particle->radius, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 20; i++) {
        glVertex3f(particle->vertex[i * 3], particle->vertex[i * 3 + 1], particle->vertex[i * 3 + 2]);
    }
    glEnd();
    glPopMatrix();
}

void render_circle_test(int width, int height) {
    float radian = (360 / NUMBER_OF_TRIANGLES) / (180.0 / M_PI);//get radian for each triangle
    float circle[3 * NUMBER_OF_TRIANGLES];//each vertex needs three values
    for (int i = 0; i < 20; i++) {
        circle[i * 3] = cosf(radian * i);//X
        circle[i * 3 + 1] = sinf(radian * i); //Y
        circle[i * 3 + 2] = 0; //Z is always 0 in 2D
    }
    glPushMatrix();
    glTranslatef(width/2 , height/2, 0.0);
    glScalef(width / 2, width / 2, 0.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 20; i++) {
        glVertex3f(circle[i * 3], circle[i * 3 + 1], circle[i * 3 + 2]);
    }
    glVertex3f(1, 0, 0); //the last vertex is the start point since we are using GL_LINE_LOOP
    glEnd();
    glPopMatrix();
}

void render_game_time(Game_Log* game_log) {
    int length_of_text = 0;
    int width_of_text = 0;
    for (char* t = game_log->total_time_prefix; *t != '\0'; t++) {
        length_of_text++;
        width_of_text += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
    for (char* t = game_log->minutes; *t != '\0'; t++) {
        length_of_text++;
        width_of_text += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
    for (char* t = game_log->seconds; *t != '\0'; t++) {
        length_of_text++;
        width_of_text += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    } 
    glColor3f(1, 1, 1);
    glRasterPos2f(game_log->width - width_of_text, game_log->height - 0.03 * game_log->height);
    for (char* t = game_log->total_time_prefix; *t != '\0'; t++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
    for (char* t = game_log->minutes; *t != '\0'; t++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
    for (char* t = game_log->seconds; *t != '\0'; t++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
}

void render_game_score(Game_Log* game_log) {
    glColor3f(1, 1, 1);
    glRasterPos2f(0, game_log->height - 0.03 * game_log->height);
    for (char* t = game_log->score_prefix; *t != '\0'; t++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
    for (char* t = game_log->score_char; *t != '\0'; t++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
}

void render_end_game_info(Game_Log* game_log) {
    int length_of_text = 0;
    int width_of_text = 0;
    for (char* t = game_log->game_over; *t != '\0'; t++) {
        length_of_text++;
        width_of_text += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
    glColor3f(1, 1, 1);
    glRasterPos2f(game_log->width/2 - width_of_text /2 , game_log->height/2);
    for (char* t = game_log->game_over; *t != '\0'; t++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *t);
    }
}
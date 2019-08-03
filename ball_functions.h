#ifndef ball_functions_h
#define ball_functions_h

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

typedef struct
{
    double poz_x;
    double poz_y;
    double n_x;
    double n_y;
    GLdouble radius;
    int alive;
} Positions;

void draw_ball(void);
void ball_light(void);
void ball_moving(int ball_id);
void check_coalition(int ball_id, int i);
void check_walls(int ball_id);
void change_direction(int ball_id, int mode);
double distance(int ball_id, int i);
void ball_divition(int ball_id);

#endif
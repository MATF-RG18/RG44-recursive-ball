#ifndef player_functions_h
#define player_functions_h

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

void draw_player(void);
void player_collision(void);
void player_moving(char c);

#endif
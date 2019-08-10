#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#include "weapon_functions.h"
#include "ball_functions.h"
#include "textures.h"

//#define WALLTEXTURE 0
//#define FLOORTEXTURE 1

//define TEXTURE_NUMBER 6

double weapon_height = -1.0;
double weapon_speed = 0.04;
double weapon_position = 0;
int weapon_fired = 0;
extern double eps;
extern Positions balls[7];

extern double player_height;

extern double left_wall, right_wall, bottom_wall, top_wall;

int balls_left = 7;
void init_weapon()
{
    weapon_height = -1.0;
    weapon_speed = 0.04;
    weapon_position = 0;
    weapon_fired = 0;
}

void fire_weapon()
{
    if (weapon_fired == 1 && weapon_height <= top_wall + eps)
    {
        glLineWidth(2.1);
        glBegin(GL_LINES);

        glColor3f(0, 0, 1);
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glVertex3f(weapon_position, -1.0 + player_height, 0);
        glVertex3f(weapon_position, weapon_height, 0);

        glEnd();
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        glLineWidth(1.f);
        weapon_height += weapon_speed;
        check_hit();
    }
    else if (weapon_fired == 1)
    {
        weapon_height = -1.0;
        weapon_fired = 0;
    }
}
void check_hit()
{
    for (int i = 0; i < 7; i++)
    {
        if (balls[i].alive == 1)
        {
            if ((weapon_height - balls[i].poz_y + balls[i].radius) >= eps && (balls[i].poz_x - balls[i].radius - weapon_position) <= eps && (balls[i].poz_x + balls[i].radius - weapon_position) >= eps)
            {
                weapon_fired = 0;
                weapon_height = -1.0;
                ball_divition(i);
                balls_left -= 1;
                break;
            }
        }
    }
}

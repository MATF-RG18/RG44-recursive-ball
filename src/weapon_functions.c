#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#include "weapon_functions.h"
#include "ball_functions.h"
#include "textures.h"

double weapon_height = -1.0;
double weapon_speed = 0.09;
double weapon_position = 0;
int weapon_fired = 0;
extern double eps;
extern Positions balls[7];

extern double player_height;
extern double player_radisu;

extern double left_wall, right_wall, bottom_wall, top_wall;

extern int level;
extern int game_success;

int balls_left = 7;
void init_weapon()
{
    weapon_height = -1.0;
    weapon_speed = 0.09;
    weapon_position = 0;
    weapon_fired = 0;
}

void fire_weapon()
{
    if (weapon_fired == 1 && weapon_height <= top_wall + eps)
    {
        glLineWidth(3.1);
        glDisable(GL_TEXTURE_2D);

        weapon_light();
        //glColor3f(0, 0, 1.0);

        glBegin(GL_LINES);
        glVertex3f(weapon_position, -1.0 + player_height, 0);
        glVertex3f(weapon_position, weapon_height, 0);

        glEnd();

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
                if (balls_left == 0)
                {
                    level += 1;
                    if (level == 4)
                    {
                        level = 0;
                        game_success = 1;
                        break;
                    }
                    /*
                    balls_left = 7;
                    init_ball();
                    init_weapon();*/
                }

                break;
            }
        }
    }
}
void weapon_light(void)
{
    GLfloat ambient_coeffs[] = {9.0 / 255., 41.0 / 255.0, 129.0 / 255.0, 1};
    GLfloat diffuse_coeffs[] = {14.0 / 255.0, 60.0 / 255.0, 185.0 / 255.0, 1};
    GLfloat specular_coeffs[] = {0.2, 0.2, 0.2, 0.2};
    GLfloat shininess = 30;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
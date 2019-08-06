#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#include "player_functions.h"
#include "ball_functions.h"

extern double player_x, player_y, player_z;
extern double player_radius;
extern double player_movement;
extern double player_height;

extern int weapon_fired;

extern double left_wall, right_wall, bottom_wall, top_wall;
extern GLuint textureNames[5];

extern Positions balls[7];
extern int animation_ongoing;
extern double eps;

#define BODYTEXTURE 3
#define HEADTEXTURE 4

void draw_player(void)
{

    GLUquadric *players_body = gluNewQuadric();
    GLUquadric *players_head = gluNewQuadric();
    GLUquadric *players_leftarm = gluNewQuadric();
    GLUquadric *players_rightarm = gluNewQuadric();
    glEnable(GL_COLOR);
    glPushMatrix();
    /*body */
    glTranslatef(player_x, player_y, player_z);
    glRotatef(90, -1, 0, 0);
    gluQuadricDrawStyle(players_body, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, textureNames[HEADTEXTURE]);
    gluQuadricTexture(players_body, GL_TRUE);
    gluQuadricNormals(players_body, GLU_SMOOTH);
    gluCylinder(players_body, player_radius, player_radius, player_height, 50, 50);

    glColor3f(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);

    /*head */
    glTranslatef(0, 0, player_height);
    gluQuadricDrawStyle(players_head, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, textureNames[HEADTEXTURE]);
    gluQuadricTexture(players_head, GL_TRUE);
    gluQuadricNormals(players_head, GLU_SMOOTH);
    gluCylinder(players_head, player_radius / 2.0, player_radius / 2.0, player_radius / 1.5, 50, 50);
    glTranslatef(0, 0, -player_height);

    /*arms */

    glTranslatef(player_radius, 0, player_height / 2.0);
    gluQuadricDrawStyle(players_rightarm, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, textureNames[HEADTEXTURE]);
    gluQuadricTexture(players_rightarm, GL_TRUE);
    gluQuadricNormals(players_rightarm, GLU_SMOOTH);
    gluCylinder(players_rightarm, player_radius / 2.0, player_radius / 2.0, player_height / 3.0, 50, 50);
    glTranslatef(-2.0 * player_radius, 0, 0);
    gluQuadricDrawStyle(players_leftarm, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, textureNames[HEADTEXTURE]);
    gluQuadricTexture(players_leftarm, GL_TRUE);
    gluQuadricNormals(players_leftarm, GLU_SMOOTH);
    gluCylinder(players_leftarm, player_radius / 2.0, player_radius / 2.0, player_height / 3.0, 50, 50);

    glPopMatrix();
    glDisable(GL_COLOR);
}
void player_moving(char c)
{
    if (weapon_fired)
    {
        return;
    }

    switch (c)
    {
    case 'a':
        if (player_x - player_movement <= (left_wall + player_radius * 2.5))
            player_x = left_wall + player_radius * 2.5;
        else
        {
            player_x -= player_movement;
        }
        break;
    case 'd':
        if (player_x + player_movement >= (right_wall - player_radius * 2.5))
            player_x = right_wall - player_radius * 2.5;
        else
        {
            player_x += player_movement;
        }
        break;
    default:
        break;
    }
}
void player_coalision(void)
{
    for (int i = 0; i < 7; i++)
    {
        if (balls[i].alive == 1)
        {
            if (((balls[i].poz_x + eps) < (player_x + player_radius + balls[i].radius)) && ((balls[i].poz_x - eps) > (player_x - player_radius - balls[i].radius)) &&
                ((balls[i].poz_y + eps) < (bottom_wall + player_height + balls[i].radius)))
            {
                animation_ongoing = 0;
            }
        }
    }
}

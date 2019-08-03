#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#include "player_functions.h"

extern double player_x, player_y, player_z;
extern double player_radius;
extern double player_movement;

extern int weapon_fired;

extern double left_wall, right_wall, bottom_wall, top_wall;

void draw_player(void)
{
    glEnable(GL_COLOR);
    glPushMatrix();
    double clip_plane[] = {0, 1, 0, 1};
    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);
    glColor4f(1, 0, 0, 0.75);
    player_light();
    glTranslatef(player_x, player_y, player_z);
    glRotatef(90, -1, 0, 0);
    player_light();
    glutSolidSphere(player_radius, 50, 50);
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
    glDisable(GL_COLOR);
}
void player_moving(char c)
{
    if(weapon_fired){
        return;
    }

    switch (c)
    {
    case 'a':
        if (player_x - player_movement <= left_wall + player_radius)
            player_x = left_wall + player_radius;
        else
        {
            player_x -= player_movement;
        }
        break;
    case 'd':
        if (player_x + player_movement >= right_wall - player_radius)
            player_x = right_wall - player_radius;
        else
        {
            player_x += player_movement;
        }
        break;
    default:
        break;
    }
}
void player_light(void)
{
    GLfloat ambient_coeffs[] = {(float)129 / 255, (float)100 / 255, (float)100 / 255, 1};
    GLfloat diffuse_coeffs[] = {(float)255 / 255, (float)100 / 255, (float)100 / 255, 1};
    GLfloat specular_coeffs[] = {1, 1, 1, 1};
    GLfloat shininess = 30;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
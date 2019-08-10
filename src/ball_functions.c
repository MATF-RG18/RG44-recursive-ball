#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#include "ball_functions.h"

#define SPHERETEXTURE 2
#define TEXTURE_NUMBER 7

Positions balls[7];
extern GLuint textureNames[TEXTURE_NUMBER];

int angle = 0;

/*ball speed*/
double speed = 30.0;
double eps = 0.0001;

extern double left_wall, right_wall, bottom_wall, top_wall;

extern int nivo;

void init_ball(void)
{

    speed = 30.0;
    if (nivo == 2)
    {
        speed = 20.0;
    }
    for (int i = 0; i < 7; i++)
    {
        balls[i].poz_x = 0;
        balls[i].poz_y = 0;
        balls[i].n_x = 0;
        balls[i].n_y = 0;
        balls[i].radius = 0;
        balls[i].alive = 0;
    }
    srand(time(NULL));
    balls[0].radius = 0.1;
    balls[0].poz_x = ((double)rand() / (double)RAND_MAX) * (right_wall - balls[0].radius) * (rand() / (double)RAND_MAX > 0.5 ? 1.0 : -1.0);
    balls[0].poz_y = ((double)rand() / (double)RAND_MAX) * (top_wall - balls[0].radius);
    double tmp_x = -(double)rand() / (double)RAND_MAX;
    double tmp_y = -1.0;
    double norm_tmp = sqrt(pow(tmp_x - balls[0].poz_x, 2) + pow(tmp_y - balls[0].poz_y, 2));
    balls[0].n_x = ((tmp_x - balls[0].poz_x) / norm_tmp) / speed;
    balls[0].n_y = ((tmp_y - balls[0].poz_y) / norm_tmp) / speed;

    if (nivo == 2)
    {
        balls[0].radius = 0.2;
    }

    balls[0].alive = 1;
}
void draw_ball(void)
{
    for (int i = 0; i < 7; i++)
    {
        if (balls[i].alive)
        {

            GLUquadricObj *quadricObj = gluNewQuadric();
            gluQuadricDrawStyle(quadricObj, GLU_FILL);
            glBindTexture(GL_TEXTURE_2D, textureNames[SPHERETEXTURE]);
            gluQuadricTexture(quadricObj, GL_TRUE);
            gluQuadricNormals(quadricObj, GLU_SMOOTH);
            glPushMatrix();
            glTranslatef(balls[i].poz_x, balls[i].poz_y, 0);
            glRotatef(angle, 1, 1, 1);
            angle += 5;
            gluSphere(quadricObj, balls[i].radius, 50, 50);
            glPopMatrix();
        }
    }
}
void ball_moving(int ball_id)
{
    balls[ball_id].poz_x += balls[ball_id].n_x;
    balls[ball_id].poz_y += balls[ball_id].n_y;
    check_walls(ball_id);
    for (int i = 0; i < 7; i++)
    {
        if (ball_id != i)
        {
            if (balls[i].alive)
            {
                check_collision(ball_id, i);
            }
        }
    }
}
void check_collision(int ball_id, int i)
{
    if ((distance(ball_id, i) - balls[ball_id].radius - balls[i].radius) <= eps)
    {
        balls[ball_id].poz_x -= eps;
        //balls[ball_id].poz_y-=eps;
        balls[i].poz_x += eps;
        // balls[i].poz_y+=eps;
        change_direction(ball_id, 0);
        change_direction(i, 0);
    }
}
void check_walls(int ball_id)
{
    if ((balls[ball_id].poz_x - (left_wall + balls[ball_id].radius)) <= eps)
    {
        balls[ball_id].poz_x = left_wall + balls[ball_id].radius + eps;
        change_direction(ball_id, 0);
    }
    else if ((balls[ball_id].poz_x - (right_wall - balls[ball_id].radius)) >= eps)
    {
        balls[ball_id].poz_x = right_wall - balls[ball_id].radius - eps;
        change_direction(ball_id, 0);
    }

    if ((balls[ball_id].poz_y - (top_wall - balls[ball_id].radius)) >= eps)
    {
        balls[ball_id].poz_y = top_wall - balls[ball_id].radius - eps;
        change_direction(ball_id, 1);
    }
    else if ((balls[ball_id].poz_y - (bottom_wall + balls[ball_id].radius)) <= eps)
    {
        balls[ball_id].poz_y = bottom_wall + balls[ball_id].radius + eps;
        change_direction(ball_id, 1);
    }
}
void change_direction(int ball_id, int mode)
{

    if (mode == 0)
    {
        balls[ball_id].n_x *= -1;
    }
    else
    {
        balls[ball_id].n_y *= -1;
    }
}
double distance(int ball_id, int i)
{
    return (double)sqrt(pow(balls[ball_id].poz_x - balls[i].poz_x, 2) + pow(balls[ball_id].poz_y - balls[i].poz_y, 2));
}
void ball_divition(int ball_id)
{
    if (balls[ball_id].alive == 1)
    {

        if (ball_id == 0)
        {
            balls[1].radius = balls[0].radius / 2.0;
            balls[2].radius = balls[0].radius / 2.0;
            balls[1].poz_x = balls[0].poz_x - balls[0].radius - eps;
            balls[2].poz_x = balls[0].poz_x + balls[0].radius + eps;
            balls[1].poz_y = balls[0].poz_y - balls[0].radius;
            balls[2].poz_y = balls[0].poz_y - balls[0].radius;
            balls[1].n_x = balls[0].n_x / 1.2;
            balls[1].n_y = fabs(balls[0].n_y / 1.2);
            balls[2].n_x = balls[0].n_x * (-1.0) / 1.2;
            balls[2].n_y = fabs(balls[0].n_y / 1.2);
            balls[0].alive = 0;
            balls[1].alive = 1;
            balls[2].alive = 1;
        }
        else if (ball_id >= 3)
        {
            balls[ball_id].alive = 0;
        }
        else if (ball_id == 1)
        {
            balls[3].radius = balls[1].radius / 2.0;
            balls[4].radius = balls[1].radius / 2.0;
            balls[3].poz_x = balls[1].poz_x - balls[1].radius - eps;
            balls[4].poz_x = balls[1].poz_x + balls[1].radius + eps;
            balls[3].poz_y = balls[1].poz_y - balls[1].radius;
            balls[4].poz_y = balls[1].poz_y - balls[1].radius;
            balls[3].n_x = balls[1].n_x / 1.2;
            balls[3].n_y = fabs(balls[1].n_y / 1.2);
            balls[4].n_x = balls[1].n_x * (-1.0) / 1.2;
            balls[4].n_y = fabs(balls[1].n_y / 1.2);
            balls[1].alive = 0;
            balls[3].alive = 1;
            balls[4].alive = 1;
        }
        else if (ball_id == 2)
        {
            balls[5].radius = balls[2].radius / 2.0;
            balls[6].radius = balls[2].radius / 2.0;
            balls[5].poz_x = balls[2].poz_x - balls[2].radius - eps;
            balls[6].poz_x = balls[2].poz_x + balls[2].radius + eps;
            balls[5].poz_y = balls[2].poz_y - balls[2].radius;
            balls[6].poz_y = balls[2].poz_y - balls[2].radius;
            balls[5].n_x = balls[2].n_x / 1.2;
            balls[5].n_y = fabs(balls[2].n_y / 1.2);
            balls[6].n_x = balls[2].n_x * (-1.0) / 1.2;
            balls[6].n_y = fabs(balls[2].n_y / 1.2);
            balls[2].alive = 0;
            balls[5].alive = 1;
            balls[6].alive = 1;
        }
    }
}
void ball_light(void)
{
    GLfloat ambient_coeffs[] = {0.0, 1, 1, 1};
    GLfloat diffuse_coeffs[] = {0.42, 1, 1, 1};
    GLfloat specular_coeffs[] = {1, 1, 1, 1};
    GLfloat shininess = 30;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
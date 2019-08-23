#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>

#include "callback_functions.h"
#include "ball_functions.h"
#include "weapon_functions.h"
#include "bitmap_functions.h"
#include "player_functions.h"
#include "textures.h"
#include "background.h"

#define TIMER_ID 0
#define TIMER_INTERVAL 50

#define WALLTEXTURE 0
#define FLOORTEXTURE 1
#define GAMEOVER 5
#define GAMESTART 6
#define NEXTLEVEL 7
#define WIN 8

#define TEXTURE_NUMBER 9

//game indicators
int level = 0;
int game_success = 0;
int game_end = 0;
int start_game = 1;

extern GLuint textureNames[TEXTURE_NUMBER];

//Ball definitions
extern Positions balls[7];
extern int balls_left;
extern double speed;

//description of a room
double left_wall = -1.79, right_wall = 1.79, bottom_wall = -1.0, top_wall = 1.0;

//player description
extern double player_x;
extern double player_movement;
extern double player_height;
extern double player_radius;

//weapon description
extern double weapon_position;
extern int weapon_fired;

//animation information
int animation_ongoing = 0;

//information about camera
float cameraX = 0, cameraY = 0, cameraZ = 2.75;

void on_keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        /*game exit*/
        glDeleteTextures(1, textureNames);
        exit(0);
        break;
    case 's':
    case 'S':
        //animation start
        if (start_game == 1)
        {
            level = 1;
            init_ball();
            init_weapon();
            glutDisplayFunc(on_display);
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);

            start_game = 0;
        }
        if (!animation_ongoing)
        {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
        }
        break;
    case 'p':
    case 'P':
        //animation pause
        if (animation_ongoing == 1)
        {
            animation_ongoing = 0;
            break;
        }
        else
        {
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            animation_ongoing = 1;
            break;
        }
    case 'r':
    case 'R':
        start_game = 1;
        game_end = 0;
        game_success = 0;
        glutDisplayFunc(display_start_screen);
        break;
    case 'a':
    case 'd':
        player_moving(key);
        break;
    case 'f':
        if (weapon_fired == 0)
        {
            weapon_position = player_x - player_radius;
            weapon_fired = 1;
        }
        break;
    case 'c':
    case 'C':
        animation_ongoing = 1;
        balls_left = 7;
        init_ball();
        init_weapon();
        glutDisplayFunc(on_display);
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        break;
    }
}

void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, width / (float)height, 1, 1000);
}
void on_timer(int value)
{
    if (value != TIMER_ID)
        return;

    for (int i = 0; i < 7; i++)
    {
        if (balls[i].alive == 1)
        {
            ball_moving(i);
        }
    }

    player_collision();

    if (game_end)
    {
        glutDisplayFunc(display_end_screen);
        //gameover - initialize game variables
        game_end = 0;
        top_wall = 1.0;
        speed = 1.0;
        start_game = 1;
        game_success = 0;
        level = 1;
        balls_left = 7;
    }
    if (game_success == 1)
    {
        //game won - initialize game variables
        game_success = 0;
        top_wall = 1.0;
        speed = 1.0;
        start_game = 1;
        level = 1;
        balls_left = 7;
        glutDisplayFunc(display_win_screen);
    }

    if (level == 3)
    {
        top_wall -= 0.001;
    }

    if (balls_left == 0)
    {
        animation_ongoing = 0;
        glutDisplayFunc(display_next_level);
    }

    if (bottom_wall + player_height >= top_wall)
    {
        game_end = 1;
    }

    glutPostRedisplay();

    if (animation_ongoing)
    {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}
void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //information about the light
    GLfloat light_position[] = {0, 0, 1, 0};
    GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1};
    GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    //light
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    //set the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
              0, 0, 0,
              0, 1, 0);

    set_texture();

    if (weapon_fired == 1)
    {
        fire_weapon();
    }

    draw_player();
    draw_ball();

    glFlush();
    glutSwapBuffers();
}
void display_win_screen(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    set_win_texture();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.8, 0, 0);
    bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(0.8, 0.8, 0.8);

    bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

    bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glFlush();
    glutSwapBuffers();
}
void display_next_level(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    set_next_level_texture();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

    bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(0.8, 0, 0);
    bitmap_output(-4.9, -2.1, " Press - c - to continue", GLUT_BITMAP_TIMES_ROMAN_24);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glFlush();
    glutSwapBuffers();
}

void display_start_screen(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    set_start_textures();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.8, 0, 0);
    bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1, 1, 1);
    bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

    bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    init_ball();

    animation_ongoing = 0;

    glFlush();
    glutSwapBuffers();
}
void display_end_screen(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    set_end_texture();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.8, 0, 0);
    bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);

    bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(0.8, 0.8, 0.8);
    bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

    bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
    bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glFlush();
    glutSwapBuffers();
}
void set_end_texture()
{
    glBindTexture(GL_TEXTURE_2D, textureNames[GAMEOVER]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 1);

    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 1);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 1);

    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 1);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
void set_win_texture()
{
    glBindTexture(GL_TEXTURE_2D, textureNames[WIN]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 1);

    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 1);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 1);

    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 1);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
void set_start_textures()
{
    glBindTexture(GL_TEXTURE_2D, textureNames[GAMESTART]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(-2, -2, 1);

    glTexCoord2f(1, 0);
    glVertex3f(2, -2, 1);

    glTexCoord2f(1, 1);
    glVertex3f(2, 2, 1);

    glTexCoord2f(0, 1);
    glVertex3f(-2, 2, 1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
void set_next_level_texture()
{
    glBindTexture(GL_TEXTURE_2D, textureNames[NEXTLEVEL]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(-2, -2, 1);

    glTexCoord2f(1, 0);
    glVertex3f(2, -2, 1);

    glTexCoord2f(1, 1);
    glVertex3f(2, 2, 1);

    glTexCoord2f(0, 1);
    glVertex3f(-2, 2, 1);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
void set_texture()
{
    //Texture binded with the area
    glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(left_wall, bottom_wall, -1);

    glTexCoord2f(1, 0);
    glVertex3f(right_wall, bottom_wall, -1);

    glTexCoord2f(1, 1);
    glVertex3f(right_wall, top_wall, -1);

    glTexCoord2f(0, 1);
    glVertex3f(left_wall, top_wall, -1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    //left wall
    glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(left_wall, bottom_wall, 1);

    glTexCoord2f(1, 0);
    glVertex3f(left_wall, bottom_wall, -1);

    glTexCoord2f(1, 1);
    glVertex3f(left_wall, top_wall, -1);

    glTexCoord2f(0, 1);
    glVertex3f(left_wall, top_wall, 1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    //right wall
    glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(right_wall, bottom_wall, -1);

    glTexCoord2f(1, 0);
    glVertex3f(right_wall, bottom_wall, 1);

    glTexCoord2f(1, 1);
    glVertex3f(right_wall, top_wall, 1);

    glTexCoord2f(0, 1);
    glVertex3f(right_wall, top_wall, -1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    //bottom
    glBindTexture(GL_TEXTURE_2D, textureNames[FLOORTEXTURE]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(left_wall, bottom_wall, 1);

    glTexCoord2f(1, 0);
    glVertex3f(right_wall, bottom_wall, 1);

    glTexCoord2f(1, 1);
    glVertex3f(right_wall, bottom_wall, -1);

    glTexCoord2f(0, 1);
    glVertex3f(left_wall, bottom_wall, -1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    //top
    glBindTexture(GL_TEXTURE_2D, textureNames[FLOORTEXTURE]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(left_wall, top_wall, 1);

    glTexCoord2f(1, 0);
    glVertex3f(right_wall, top_wall, 1);

    glTexCoord2f(1, 1);
    glVertex3f(right_wall, top_wall, -1);

    glTexCoord2f(0, 1);
    glVertex3f(left_wall, top_wall, -1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}
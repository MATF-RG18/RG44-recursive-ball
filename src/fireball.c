#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "ball_functions.h"
#include "weapon_functions.h"
#include "player_functions.h"
#include "textures.h"
#include "background.h"
#include "bitmap_functions.h"

#define TIMER_ID 0
#define TIMER_INTERVAL 50

#define WALLTEXTURE 0
#define FLOORTEXTURE 1
#define GAMEOVER 5
#define GAMESTART 6

#define TEXTURE_NUMBER 7

int nivo = 0;
int game_success = 0;

extern GLuint textureNames[TEXTURE_NUMBER];

extern Positions balls[7];
extern int balls_left;
extern double speed;

/*speed of ball and epsilon distance from walls and other balls*/
double left_wall = -1.79, right_wall = 1.79, bottom_wall = -1.0, top_wall = 1.0;

/*player coordinates*/
extern double player_x;
extern double player_movement;
extern double player_height;

/*describe weapon*/
extern double weapon_position;
extern int weapon_fired;

/*animation information*/
int animation_ongoing = 0;

/*information about camera*/
static float cameraX = 0, cameraY = 0, cameraZ = 2.75;

int game_end = 0;
int start_game = 1;

/*callback function*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void display_start_screen(void);
static void display_end_screen(void);
static void display_next_level(void);
static void on_timer(int value);

//static void initialize_texture(void);
static void set_texture(void);
static void set_end_texture(void);
static void set_start_textures();

int main(int argc, char **argv)
{
  /*information about the light*/
  GLfloat light_position[] = {0, 0, 1, 0};
  GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1};
  GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
  GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

  /*light*/
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  /*glut init*/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  /*creating a window*/
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Recursive-ball");

  glutFullScreen();

  /*callback functions*/
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(display_start_screen);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);

  init_ball();

  animation_ongoing = 0;

  initialize_textures();

  glutMainLoop();

  return 0;
}
static void on_keyboard(unsigned char key, int x, int y)
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
    /*animation start*/
    if (start_game == 1)
    {
      nivo = 1;
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
    /*animation end*/
    if (animation_ongoing == 1)
    {
      animation_ongoing = 0;
    }
    else
    {
      animation_ongoing = 1;
      glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
    break;
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
      weapon_position = player_x;
      weapon_fired = 1;
    }
    break;
  case 'n':
  case 'N':
    animation_ongoing = 1;
    balls_left = 7;
    init_ball();
    init_weapon();
    glutDisplayFunc(on_display);
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    break;
  }
}

static void on_reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, width / (float)height, 1, 1000);
}
static void on_timer(int value)
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
    game_end = 0;
    top_wall = 1.0;
    speed = 30.0;
    start_game = 1;
    game_success = 0;
    nivo = 1;
    balls_left = 7;
  }
  if (game_success == 1)
  {
    game_success = 0;
    glutDisplayFunc(display_start_screen);
    top_wall = 1.0;
    speed = 30.0;
    start_game = 1;
    nivo = 1;
    balls_left = 7;
  }

  if (nivo == 3)
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
static void on_display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //*set the camera*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraX, cameraY, cameraZ,
            0, 0, 0,
            0, 1, 0);

  //draw_coordinate_system();
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
static void display_next_level(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 5,
            0, 0, 0,
            0, 1, 0);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glColor3f(1, 1, 1);
  bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

  bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);

  glColor3f(0.7, 0.7, 0.7);
  bitmap_output(-1.0, 0.0, "PRESS - N - TO CONTINUE", GLUT_BITMAP_TIMES_ROMAN_24);

  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);

  glFlush();
  glutSwapBuffers();
}

static void display_start_screen(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 5,
            0, 0, 0,
            0, 1, 0);

  set_start_textures();

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glColor3f(1, 1, 1);
  bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

  bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);

  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);

  glFlush();
  glutSwapBuffers();
}
static void display_end_screen(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 5,
            0, 0, 0,
            0, 1, 0);

  set_end_texture();
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.8, 0.8, 0.8);
  bitmap_output(-4.9, -2.3, " Press - s - for start ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, -2.5, " Press - esc - for exit ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, -2.7, " Press - p - to pause the game ", GLUT_BITMAP_TIMES_ROMAN_24);

  bitmap_output(-4.9, 2.7, " Press - f - to fire   ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, 2.5, " Press - a - to go left ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-4.9, 2.3, " Press - d- to go right ", GLUT_BITMAP_TIMES_ROMAN_24);

  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glFlush();
  glutSwapBuffers();
}
static void set_end_texture()
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
}
static void set_start_textures()
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
}
static void set_texture()
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

  //left wall
  glBindTexture(GL_TEXTURE_2D, 0);
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

  //right wall
  glBindTexture(GL_TEXTURE_2D, 0);
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
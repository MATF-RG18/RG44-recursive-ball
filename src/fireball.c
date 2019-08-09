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

int nivo;

extern GLuint textureNames[5];

extern Positions balls[7];
extern int balls_left;

int screen_help = 0;

/*speed of ball and epsilon distance from walls and other balls*/
double left_wall = -1.79, right_wall = 1.79, bottom_wall = -1.0, top_wall = 1.0;

/*player coordinates*/
extern double player_x;
extern double player_movement;

/*describe weapon*/
extern double weapon_position;
extern int weapon_fired;

/*functions for scene
static void draw_coordinate_system(void);
*/
//static void screen_light(void);

/*animation information*/
int animation_ongoing = 0;

/*information about camera*/
static float cameraX = 0, cameraY = 0, cameraZ = 2.75;

/*callback function*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void display_start_screen(void);
static void on_timer(int value);

//static void initialize_texture(void);
static void set_texture(void);

int main(int argc, char **argv)
{
  /*information about the light*/
  GLfloat light_position[] = {0, 0, 1, 0};
  GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1};
  GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
  GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

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

  /*light*/
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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
  case 'g':
  case 'G':
    /*animation start*/
    if (!animation_ongoing)
    {
      glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
      animation_ongoing = 1;
    }
    break;
  case 's':
  case 'S':
    /*animation end*/
    animation_ongoing = 0;
    break;
  case 'r':
  case 'R':
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
  case '1':
    nivo = 1;
    glutDisplayFunc(on_display);
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    break;
  case '2':
    nivo = 2;
    glutDisplayFunc(on_display);
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    break;
  case '3':
    nivo = 3;
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
static void display_start_screen(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 10,
            0, 0, 0,
            0, 10, 0);

  glTranslatef(0, -1.5, 0);
  glColor3f(1, 0, 1);
  glLineWidth(GL_LINE_LOOP);
  glVertex2f(-3, 2);
  glVertex2f(3, 2);
  glVertex2f(3, 0.5);
  glVertex2f(-3, 0.5);
  glEnd();

  glColor3f(1, 0, 0);
  bitmap_output(-1.0, 1.7, "Select level: ", GLUT_BITMAP_TIMES_ROMAN_24);

  bitmap_output(-1.0, 1.2, "1 - nivo  ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-1.0, 0.7, "2 - nivo ", GLUT_BITMAP_TIMES_ROMAN_24);
  bitmap_output(-1.0, 0.2, "3 - nivo ", GLUT_BITMAP_TIMES_ROMAN_24);

  glTranslatef(0, 1.5, 0);

  glFlush();
  glutSwapBuffers();
}

static void set_texture()
{
  //Texture binded with the area
  glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 0);
  glVertex3f(left_wall, -1, -1);

  glTexCoord2f(1, 0);
  glVertex3f(right_wall, -1, -1);

  glTexCoord2f(1, 1);
  glVertex3f(right_wall, 1, -1);

  glTexCoord2f(0, 1);
  glVertex3f(left_wall, 1, -1);
  glEnd();

  //left wall
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 0);
  glVertex3f(left_wall, -1, 1);

  glTexCoord2f(1, 0);
  glVertex3f(left_wall, -1, -1);

  glTexCoord2f(1, 1);
  glVertex3f(left_wall, 1, -1);

  glTexCoord2f(0, 1);
  glVertex3f(left_wall, 1, 1);
  glEnd();

  //right wall
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 0);
  glVertex3f(right_wall, -1, -1);

  glTexCoord2f(1, 0);
  glVertex3f(right_wall, -1, 1);

  glTexCoord2f(1, 1);
  glVertex3f(right_wall, 1, 1);

  glTexCoord2f(0, 1);
  glVertex3f(right_wall, 1, -1);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  //bottom
  glBindTexture(GL_TEXTURE_2D, textureNames[FLOORTEXTURE]);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 0);
  glVertex3f(left_wall, -1, 1);

  glTexCoord2f(1, 0);
  glVertex3f(right_wall, -1, 1);

  glTexCoord2f(1, 1);
  glVertex3f(right_wall, -1, -1);

  glTexCoord2f(0, 1);
  glVertex3f(left_wall, -1, -1);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  //top
  glBindTexture(GL_TEXTURE_2D, textureNames[FLOORTEXTURE]);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, 1);

  glTexCoord2f(0, 0);
  glVertex3f(left_wall, 1, 1);

  glTexCoord2f(1, 0);
  glVertex3f(right_wall, 1, 1);

  glTexCoord2f(1, 1);
  glVertex3f(right_wall, 1, -1);

  glTexCoord2f(0, 1);
  glVertex3f(left_wall, 1, -1);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
}
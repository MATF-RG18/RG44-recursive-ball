#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#include "ball_functions.h"
#include "weapon_functions.h"
#include "player_functions.h"
#include "background.h"


#define TIMER_ID 0
#define TIMER_INTERVAL 50

static GLuint textureNames[2];

Positions balls[7];

int i = 0;

/*speed of ball and epsilon distance from walls and other balls*/
double speed = 10;
double eps = 0.0001;
double left_wall = -1.79, right_wall = 1.79, bottom_wall = -1.0, top_wall = 1.0;

/*player coordinates*/
double player_x = 0, player_y = -1, player_z = 0;
double player_radius = 0.075;
double player_movement = 0.05;

/*describe weapon*/
double weapon_height = -1.0;
double weapon_speed = 0.02;
double weapon_position = 0;
int weapon_fired = 0;

/*functions for scene*/
static void draw_coordinate_system(void);
static void screen_light(void);

/*animation information*/
static int animation_ongoing = 0;

/*information about camera*/
static float cameraX = 0, cameraY = 0, cameraZ = 2.75;

/*callback function*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

static void initialize_texture(void);
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
  glutDisplayFunc(on_display);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  /*light*/
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  /*initials positions of balls*/
  for (int i = 0; i < 7; i++)
  {
    balls[i].poz_x = 0;
    balls[i].poz_y = 0;
    balls[i].n_x = 0;
    balls[i].n_y = 0;
    balls[i].radius = 0;
    balls[i].alive = 0;
  }
  /*position of a main ball*/
  srand(time(NULL));
  balls[0].poz_x = ((double)rand() / (double)RAND_MAX) * 1.70 * (rand() / (double)RAND_MAX > 0.5 ? 1.0 : -1.0);
  balls[0].poz_y = ((double)rand() / (double)RAND_MAX);
  double tmp_x = -(double)rand() / (double)RAND_MAX;
  double tmp_y = -1.0;
  double norm_tmp = sqrt(pow(tmp_x - balls[0].poz_x, 2) + pow(tmp_y - balls[0].poz_y, 2));
  balls[0].n_x = ((tmp_x - balls[0].poz_x) / norm_tmp) / speed;
  balls[0].n_y = ((tmp_y - balls[0].poz_y) / norm_tmp) / speed;
  balls[0].radius = 0.2;
  balls[0].alive = 1;

  animation_ongoing = 0;

  initialize_texture();

  glutMainLoop();

  return 0;
}

static void initialize_texture()
{
  Background *background;

  glEnable(GL_TEXTURE_2D);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  background = background_init(0, 0);
  char* fileName1="window5.bmp";
  char* fileName2="concrete2.bmp";
 
 
  background_read(background, fileName1);
  glGenTextures(2, textureNames);

  glBindTexture(GL_TEXTURE_2D, textureNames[0]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //Iskljucujemo aktivnu teksturu
  glBindTexture(GL_TEXTURE_2D, 0);

  /*buttom*/
  background_read(background, fileName2);
  glBindTexture(GL_TEXTURE_2D, textureNames[1]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

    //Unistava se objekat za itanje tekstura iz fajla
  background_done(background);
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

  draw_coordinate_system();
  set_texture();
 

  if (weapon_fired == 1)
  {
    fire_weapon();
  }

  draw_player();
  draw_ball();

  glutSwapBuffers();
}
static void draw_coordinate_system(void)
{
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glColor3f((float)230 / 255, (float)225 / 255, (float)225 / 255);
  screen_light();
  glScalef(1.79f, 1.0f, 1.0f);
  glutWireCube(2);
  glPopMatrix();
  /* glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(1.79f,0,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
  glEnd();*/
  glEnable(GL_LIGHTING);
}
static void screen_light(void)
{
  GLfloat diffuse_coeffs[] = {(float)247 / 255, (float)242 / 255, (float)239 / 255, 1};
  GLfloat ambient_coeffs[] = {(float)124 / 255, (float)121 / 255, (float)120 / 255, 1};
  GLfloat specular_coeffs[] = {1, 1, 1, 1};
  GLfloat shininess = 30;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
static void set_texture(){
 /*Kreiramo povrsinu po kojoj ce biti zalepljena tekstura
  povezujemo odgovarajuce coskove citave povrsine sa coskovima teksture*/
  glBindTexture(GL_TEXTURE_2D, textureNames[0]);
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

  //levi zid
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, textureNames[0]);
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

  //desni zid
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, textureNames[0]);
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

  /*bottom*/
  glBindTexture(GL_TEXTURE_2D, textureNames[1]);
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

  /*top*/
  glBindTexture(GL_TEXTURE_2D, textureNames[1]);
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
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "callback_functions.h"
#include "ball_functions.h"
#include "weapon_functions.h"
#include "textures.h"

int main(int argc, char **argv)
{

  //glut init
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  //creating a window
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Recursive ball");

  glutFullScreen();

  //callback functions
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(display_start_screen);

  initialize_textures();

  glutMainLoop();

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "background.h"
#include "textures.h"

extern GLuint textureNames;

extern int left_wall,right_wall; 

void initialize_textures()
{
  Background *background;

  glClearColor(0, 0, 0, 0);
  glEnable(GL_TEXTURE_2D);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  background = background_init(0, 0);
  char* fileName="window5.bmp";

  background_read(background, fileName);

  glGenTextures(1, &textureNames);

  glBindTexture(GL_TEXTURE_2D, textureNames);
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

  //Unistava se objekat za itanje tekstura iz fajla
  background_done(background);
}
void set_texture(){
  glBindTexture(GL_TEXTURE_2D, textureNames);
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
/*
  //levi zid
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D, textureNames);
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
  glBindTexture(GL_TEXTURE_2D, textureNames);
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
  glEnd();*/

  glBindTexture(GL_TEXTURE_2D, 0);

}
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "background.h"
#include "textures.h"

#define WALLTEXTURE 0
#define FLOORTEXTURE 1
#define SPHERETEXTURE 2
#define BODYTEXTURE 3
#define HEADTEXTURE 4
#define GAMEOVER 5

#define TEXTURE_NUMBER 6

GLuint textureNames[TEXTURE_NUMBER];

extern int left_wall, right_wall;

void initialize_textures()
{
  Background *background;

  glEnable(GL_TEXTURE_2D);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  background = background_init(0, 0);
  char *fileName1 = "textures/wall.bmp";
  char *fileName2 = "textures/floor.bmp";
  char *fileName3 = "textures/ball_texture.bmp";
  char *fileName4 = "textures/body_texture.bmp";
  char *fileName5 = "textures/head_texture.bmp";
  char *fileName6 = "textures/game_over.bmp";

  background_read(background, fileName1);
  glGenTextures(6, textureNames);

  glBindTexture(GL_TEXTURE_2D, textureNames[WALLTEXTURE]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //Disable active texture
  //glBindTexture(GL_TEXTURE_2D, 0);

  /*bottom*/
  background_read(background, fileName2);
  glBindTexture(GL_TEXTURE_2D, textureNames[FLOORTEXTURE]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //glBindTexture(GL_TEXTURE_2D, 0);

  /*sphere texture */
  background_read(background, fileName3);
  glBindTexture(GL_TEXTURE_2D, textureNames[SPHERETEXTURE]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //glBindTexture(GL_TEXTURE_2D, 0);

  /*body texture */
  background_read(background, fileName4);
  glBindTexture(GL_TEXTURE_2D, textureNames[BODYTEXTURE]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //glBindTexture(GL_TEXTURE_2D, 0);

  /*arm textures */
  background_read(background, fileName5);
  glBindTexture(GL_TEXTURE_2D, textureNames[HEADTEXTURE]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //glBindTexture(GL_TEXTURE_2D, 0);

  /*gameover textures */
  background_read(background, fileName6);
  glBindTexture(GL_TEXTURE_2D, textureNames[GAMEOVER]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);
  //glBindTexture(GL_TEXTURE_2D, 0);

  //glDisable(GL_TEXTURE_2D);

  //delete object that reads texture from file
  background_done(background);
}

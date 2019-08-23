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
#define GAMESTART 6
#define NEXTLEVEL 7
#define WIN 8

#define TEXTURE_NUMBER 9

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
  char *fileName5 = "textures/head.bmp";
  char *fileName6 = "textures/game_over.bmp";
  char *fileName7 = "textures/start_game.bmp";
  char *fileName8 = "textures/next_level.bmp";
  char *fileName9 = "textures/you_win.bmp";

  background_read(background, fileName1);
  glGenTextures(8, textureNames);

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

  /*head textures */
  background_read(background, fileName5);
  glBindTexture(GL_TEXTURE_2D, textureNames[HEADTEXTURE]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_CLAMP);
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

  //gamestart
  background_read(background, fileName7);
  glBindTexture(GL_TEXTURE_2D, textureNames[GAMESTART]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //next level texture
  background_read(background, fileName8);
  glBindTexture(GL_TEXTURE_2D, textureNames[NEXTLEVEL]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  //win texture
  background_read(background, fileName9);
  glBindTexture(GL_TEXTURE_2D, textureNames[WIN]);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               background->width, background->height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, background->pixels);

  glBindTexture(GL_TEXTURE_2D, 0);

  //delete object that reads texture from file
  background_done(background);
}

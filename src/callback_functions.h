#ifndef callback_functions_h
#define callback_functions_h

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <assert.h>

void on_keyboard(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_timer(int value);
void on_display(void);
void display_next_level(void);
void display_start_screen(void);
void display_end_screen(void);
void set_end_texture();
void set_start_textures();
void set_texture();

#endif
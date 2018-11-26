#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 50

/*main ball*/
static GLdouble radius=0.2;
static double poz_x, poz_y, poz_z=0;
static double n_x,n_y;
static double speed=10;

/*level1 ball*/
static GLdouble radius_lv1=0;
static double poz_lv1_x=0, poz_lv1_y=0, poz_lv1_z=0;
static double n_lv1_x=0,n_lv1_y=0;

/*level2 ball*/
static GLdouble radius_lv2=0;
static double poz_lv2_x=0, poz_lv2_y=0, poz_lv2_z=0;
static double n_lv2_x=0,n_lv2_y=0;

/*level3 ball*/
static GLdouble radius_lv3=0;
static double poz_lv3_x=0, poz_lv3_y=0, poz_lv3_z=0;
static double n_lv3_x=0,n_lv3_y=0;

static void draw_ball(void);
static void ball_light(void);
static void ball_moving(double* position_x, double* position_y, double* vector_x, double* vector_y, GLdouble* r, int id);

/*player coordinates*/
static double player_x=0, player_y=-1, player_z=0;
static double player_radius=0.075;
static double player_height=0.2;

static void draw_player(void);
static void player_light(void);

static void draw_coordinate_system(void);
static void screen_light(void);

/*animation information*/
static int animation_ongoing=0;

/*information about camera*/
static float cameraX=0, cameraY=0, cameraZ=2.75;

/*callback function*/
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

int main(int argc, char **argv){
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

  
  /*position of a main ball*/
  poz_x = (double)rand()/(double)RAND_MAX;
  poz_y = (double)rand()/(double)RAND_MAX;
  double tmp_x = -(double)rand()/(double)RAND_MAX;
  double tmp_y = -1.0;
  double norm_tmp=sqrt(pow(tmp_x-poz_x, 2)+pow(tmp_y-poz_y, 2));
  n_x = ((tmp_x-poz_x)/norm_tmp)/speed;
  n_y = ((tmp_y-poz_y)/norm_tmp)/speed;

  animation_ongoing=0;

  glutMainLoop();

  return 0;
}
static void on_keyboard(unsigned char key, int x, int y){
  switch (key) {
    case 27:
      /*game exit*/
      exit(0);
      break;
    case 'g':
    case 'G':
        /*animation start*/
        if(!animation_ongoing){
          glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
          animation_ongoing=1;
        }
        break;
    case 's':
    case 'S':
        /*animation end*/
        animation_ongoing=0;
        break;
    case '1':
        radius/=2.0;
        radius_lv1=radius;
        poz_lv1_x = poz_x-radius;
        poz_lv1_y = poz_y-radius;
        poz_x+=radius;
        poz_y+=radius;
        n_lv1_x = -n_x;
        n_lv1_y=n_y;
        break;
    case '2':
        radius/=2.0;
        radius_lv2=radius;
        poz_lv2_x=poz_x-radius;
        poz_lv2_y=poz_y-radius;
        poz_lv1_x+=radius;
        poz_lv1_y+=radius;
        n_lv2_x = -n_x;
        n_lv2_y=n_y;
        break;
    case '3':
        radius_lv1/=2.0;
        radius_lv3=radius_lv1;
        poz_lv3_x = poz_lv1_x-radius_lv1;
        poz_lv3_y = poz_lv1_y-radius_lv1;
        poz_lv1_x+=radius_lv1;
        poz_lv1_y+=radius_lv1;
        n_lv3_x = -n_lv1_x;
        n_lv3_y=n_lv1_y;
        break;
    
  }
}
static void ball_moving(double* position_x, double* position_y, double* vector_x, double* vector_y, GLdouble* r, int id){
  
  *position_x += *vector_x;
  if(*position_x <= -(1-*r)|| *position_x>= (1-*r)){
    *vector_x *=-1;
  }

  *position_y +=*vector_y;
  if(*position_y<= -(1-*r) || *position_y>= (1-*r)){
    *vector_y*=-1;
  }
  /*TODO: check collision */

      if(id != 0 && sqrt(pow(*position_x-poz_x, 2)+pow(*position_y-poz_y, 2))<(*r+radius)){
        n_x*=-1;
        // n_y*=-1;
        *vector_x=*vector_x * (-1);
        //*vector_y=*vector_y * (-1);
      }
     
  
      if(id != 1 && sqrt(pow(*position_x-poz_lv1_x, 2)+pow(*position_y-poz_lv1_y, 2))<(*r+radius_lv1)){
        n_lv1_x*=-1;
        // n_lv1_y*=-1;
        *vector_x=*vector_x * (-1);
        // *vector_y=*vector_y * (-1);
      }

        if(id != 2 && sqrt(pow(*position_x-poz_lv2_x, 2)+pow(*position_y-poz_lv2_y, 2))<(*r+radius_lv2)){
          n_lv2_x*=-1;
          // n_lv2_y*=-1;
          *vector_x=*vector_x * (-1);
          // *vector_y=*vector_y * (-1);
        }

        if(id != 3 && sqrt(pow(*position_x-poz_lv3_x, 2)+pow(*position_y-poz_lv3_y, 2))<(*r+radius_lv3)){
          n_lv3_x*=-1;
          // n_lv3_y*=-1;
          *vector_x=*vector_x * (-1);
          // *vector_y=*vector_y * (-1);
        }
        

}
static void on_reshape(int width, int height){
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, width/(float)height,1, 1000);
}
static void on_timer(int value){
  if(value!=TIMER_ID)
    return;

  ball_moving(&poz_x, &poz_y, &n_x, &n_y, &radius, 0);
  ball_moving(&poz_lv1_x, &poz_lv1_y, &n_lv1_x, &n_lv1_y, &radius_lv1, 1);
  ball_moving(&poz_lv2_x, &poz_lv2_y, &n_lv2_x, &n_lv2_y, &radius_lv2, 2);
  ball_moving(&poz_lv3_x, &poz_lv3_y, &n_lv3_x, &n_lv3_y, &radius_lv3, 3);
  

  glutPostRedisplay();

  if(animation_ongoing){
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
  }

}
static void on_display(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //*set the camera*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cameraX, cameraY, cameraZ,
            0, 0, 0,
            0, 1, 0);

  
  draw_coordinate_system();
  draw_player();
  draw_ball();

  glutSwapBuffers();

}
static void draw_coordinate_system(void){
  glDisable(GL_LIGHTING);
  glColor3f((float)230/255, (float)225/255, (float)225/255);
  screen_light();
  glutWireCube(2); 
  glEnable(GL_LIGHTING);  
} 
static void draw_player(void){
  glPushMatrix();
    glColor4f(1, 0, 0, 0.75);
    player_light();
    glTranslatef(player_x, player_y, player_z);
    glRotatef(90, -1, 0, 0);
    glutSolidCone(player_radius, player_height, 50, 50);
  glPopMatrix();
}
static void draw_ball(void){
  /*main ball*/ 
  glPushMatrix();
    glColor4f(0, 1, 1, 0.75);
    ball_light();
    glTranslatef(poz_x, poz_y, poz_z);
    glutSolidSphere(radius, 50, 50);
  glPopMatrix();

  /*level1 ball*/
  glPushMatrix();
    glColor4f(0, 1, 1, 0.75);
    ball_light();
    glTranslatef(poz_lv1_x, poz_lv1_y, poz_lv1_z);
    glutSolidSphere(radius_lv1, 50, 50);
  glPopMatrix();

  /*level2 ball*/
  glPushMatrix();
    glColor4f(0, 1, 1, 0.75);
    ball_light();
    glTranslatef(poz_lv2_x, poz_lv2_y, poz_lv2_z);
    glutSolidSphere(radius_lv2, 50, 50);
  glPopMatrix();

  /*level3 ball*/
  glPushMatrix();
    glColor4f(0, 1, 1, 0.75);
    ball_light();
    glTranslatef(poz_lv3_x, poz_lv3_y, poz_lv3_z);
    glutSolidSphere(radius_lv3, 50, 50);
  glPopMatrix();


}
static void ball_light(void){
  GLfloat ambient_coeffs[] = {0.0, 1, 1, 1};
  GLfloat diffuse_coeffs[] = {0.42, 1, 1, 1};
  GLfloat specular_coeffs[] = {1, 1, 1, 1};
  GLfloat shininess = 30;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);

}
static void player_light(void){
  GLfloat ambient_coeffs[] = {(float)170/255, (float)160/255, (float)166/255, 1};
  GLfloat diffuse_coeffs[] = {(float)129/255, (float)121/255, (float)105/255, 1};
  GLfloat specular_coeffs[] = {1, 1, 1, 1};
  GLfloat shininess = 30;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
static void screen_light(void){
  GLfloat ambient_coeffs[] = {(float)247/255, (float)242/255, (float)239/255, 1};
  GLfloat diffuse_coeffs[] = {(float)124/255, (float)121/255, (float)120/255, 1};
  GLfloat specular_coeffs[] = {1, 1, 1, 1};
  GLfloat shininess = 30;


  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}


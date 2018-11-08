#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>


#define TIMER_ID 0
#define TIMER_INTERVAL 50
//precnik pocetne sfere
static GLdouble radius;

static float rotation_time;
static int timer_active;

//tekuce koordinate centra lopte i komponente vektora brzine
static float x_curr, y_curr;
static float v_x, v_y;

static int window_width, window_height;

//callback funkcije
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

//osvetljenje lopte
static void ball_light(void);
//osvetljenje sobe
//static void room_light(void);

int main(int argc, char **argv){

  //pozicija svetla
  GLfloat light_position[] = {1, 1, 1, 0};
  //ambijentalna boja svetla
  GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1};
  //difuzna boja svetla
  GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
  //spekularna boja svetla
  GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

  //inicijalizuje se glut
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  //kreira se prozor
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Recursive-ball");

  //registruju se callback funckije
  glutKeyboardFunc(on_keyboard);
  glutReshapeFunc(on_reshape);
  glutDisplayFunc(on_display);


  radius=80;
  rotation_time=0;
  timer_active=0;

  //inicijalizuje se seed za random brojeve
   srand(time(NULL));

   //inicijalizuju se globalne promenlive. Nasumicno ostavljamo pocetnu poziciju i pocetnu brzinu
   x_curr = -(window_width/2 - radius) + (2*window_width - radius) * rand() / (float) RAND_MAX;
   y_curr = -(window_height/2 - radius) + (2*window_height - radius) * rand() / (float) RAND_MAX;

   v_x = -radius/2 + radius*rand() / (float) RAND_MAX;
   v_y = -radius/2 + radius*rand() / (float) RAND_MAX;

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);

  //postavljaju se svojstva svetla
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  //obavlja se opengl inicijalizacija
  glClearColor(0, 0, 0.187, 0);

  //program ulazi u glavnu petlju
  glutMainLoop();


  return 0;
}
static void on_keyboard(unsigned char key, int x, int y){
  switch (key) {
    case 27:
      //zavrsava se program
      exit(0);
      break;
    case 'g':
    case 'G':
        //pokrece se animacija rotacije
        if(!timer_active){
          glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
          timer_active=1;
        }
        break;
    case 's':
    case 'S':
        //zaustavlja se simulacija rotacije
        timer_active=0;
        break;
  }

}
static void on_timer(int value){
  //proverava se da li callback dolazi od ogvarajuceg tajmera
  if(value != TIMER_ID){
    return;
  }
  //azurira se rotacija
  rotation_time+=18;

   //odredjivanje nove pozicije centra lopte

   x_curr += v_x;
   if(x_curr <= -(window_width - radius) || x_curr >= window_width - radius){
      v_x *= -1;
  }

  y_curr += v_y;
  if(y_curr <= -(window_height - radius) || y_curr >= window_height - radius){
      v_y *= -1;
  }

  //poziva se isrtavanje prozora
  glutPostRedisplay();

  //po potrebi se ponovo postavlja tajmer
  if(timer_active)
    glutTimerFunc(50, on_timer, 0);

}


static void on_reshape(int width, int height){
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, width/(float)height, 1, 1500);
}
static void on_display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //podesavanje pogleda
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1000,
              0, 0, 0,
              0, 1, 0);


      float ball_rotation;

      ball_rotation =360*rotation_time/(15*24);

      //transliranje lopte na novu poziciju
      //odredjenju vektorom kretanja
    //  ball_light();
      glTranslatef(x_curr, y_curr, 0);

      //rotacija lopte oko svoje ose
     glPushMatrix();
        glRotatef(ball_rotation, 0, 0, 1);
        glColor3f(0.4, 1 , 1);
        ball_light();
        glutSolidSphere(radius, 50, 50);
      glPopMatrix();
    glutSwapBuffers();
}
static void ball_light(void){
  //koeficijenti ambijlentalne refleksije materijala
  GLfloat ambient_coeffs[] = {0.0, 0.5, 0.5, 1};

  //koeficijenti difuzne refleksije materijala
  GLfloat diffuse_coeffs[] = {0.42, 1, 1, 1};

  //koeficijenti spekularne refleksije materijala
  GLfloat specular_coeffs[] = {1, 1, 1, 1};

  //koeficijent glatkosti materijala
  GLfloat shininess = 30;

  //svojstva materijala

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);

}

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
#include <GLUT/glut.h>
using namespace std;

void reshape(int width,int height);
void display();
void renderScene();
void keyboardPressed(unsigned char key, int x, int y);

int divergenceTest1(double x, double y, int limit);
int divergenceTest2(double x, double y, int limit);
void colorScheme(int n);
double getX(int x);
double getY(int y);

/*
 * stanje sustava
 */
int width, height;
int maxLimit;
double eps;
double umin, umax, vmin, vmax;
double Cx, Cy;

int typeOfService;

int main(int argc,char **argv){
  width = 500; height = 500;
  typeOfService = 0;

  printf("Unesite prag eps: "); scanf("%lf", &eps);
  printf("Unesite maksimalan broj iteracija: "); scanf("%d", &maxLimit);
  printf("Unesite podrucje kompleksne ravnine koja se promatra (umin, umax), (vmin, vmax): ");
  scanf("%lf%lf%lf%lf", &umin, &umax, &vmin, &vmax);
  printf("Unesite konstantu c: "); scanf("%lf%lf", &Cx, &Cy);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Zadatak 8");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboardPressed);
  glutMainLoop();
}

void display(){
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  // crtanje scene
  renderScene();
  glutSwapBuffers();
}

void reshape(int w,int h){
  width = w;
  height = h;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width-1, height-1, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

int divergenceTest1(double x, double y, int limit) {
  int k = -1;
  double cx = x, cy = y;
  double z1x, z1y;
  double z0x = 0, z0y = 0;

  double r;

  while (1) {
    r = sqrt(z0x * z0x + z0y * z0y);

    if (r >= eps || k >= maxLimit) break;

    k = k + 1;

    z1x = z0x * z0x - z0y * z0y + cx;
    z1y = 2 * z0x * z0y + cy;

    z0x = z1x;
    z0y = z1y;
  }

  return k;
}

int divergenceTest2(double x, double y, int limit) {
  int k = -1;
  double z1x, z1y;
  double z0x = x, z0y = y;

  double r;

  while (1) {
    r = sqrt(z0x * z0x + z0y * z0y);

    if (r >= eps || k >= maxLimit) break;

    k = k + 1;

    z1x = z0x * z0x - z0y * z0y + Cx;
    z1y = 2 * z0x * z0y + Cy;

    z0x = z1x;
    z0y = z1y;
  }

  return k;
}
void renderScene(){
  glPointSize(1.0f);

  glBegin(GL_POINTS);
  // za svaku tocku zaslona
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      double u0 = getX(i);
      double v0 = getY(j);

      int k;

      if (typeOfService == 0) {
        k = divergenceTest1(u0, v0, maxLimit);
      } else {
        k = divergenceTest2(u0, v0, maxLimit);
      }

      colorScheme(k);
      glVertex2i(i, j);

    }
  }
  glEnd();
}

double getX(int x) {
  return x * (umax - umin) / double(width) + umin;
}

double getY(int y) {
  return y * (vmax - vmin) / double(height) + vmin;
}

void colorScheme(int n) {

  /*
  if (n >= maxLimit) {
    glColor3f(0.0f, 0.0f, 0.0f);
  } else {
    glColor3f(1.0f, 1.0f, 1.0f);
  }
  return;
  */

  if (n == -1) {
    glColor3f(0.0f, 0.0f, 0.0f);
  } else if (maxLimit < 16) {
    int r = (int)((n - 1) / double(maxLimit - 1) * 255 + 0.5);
    int g = 255 - r;
    int b = ((n - 1) % (maxLimit / 2)) * 255 / (maxLimit / 2);

    glColor3f(r / 255.f, g / 255.f, b / 255.f);
  } else {
    int limit = maxLimit < 32 ? maxLimit : 32;
    int r = (n - 1) * 255 / limit;
    int g = ((n - 1) % (limit / 4)) * 255 / (limit / 4);
    int b = ((n - 1) % (limit / 8)) * 255 / (limit / 8);

    glColor3f(r / 255.f, g / 255.f, b / 255.f);
  }
}

void keyboardPressed(unsigned char key, int x, int y) {
  switch (key) {
    case 'p':
      typeOfService = 1 - typeOfService;
      break;
    default:
      break;
  }

  glutPostRedisplay();
}

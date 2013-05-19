#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <err.h>
#include <GLUT/glut.h>

using namespace std;

void reshape(int width, int height);
void display();
void renderScene();

int main(int argc, char **argv) {
  /*
   * Inicijalizacija GLUT-a
   */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(640, 480);

  glutInitWindowPosition(0, 0);
  glutCreateWindow("Zadatak 6.");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}

void display() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // crtaj scenu
  renderScene();

  glutSwapBuffers();
}

void reshape(int width, int height) {
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
}

void renderScene() {
  glPointSize(1.0f);
  glColor3f(0.0f, 0.0f, 0.0f);

  return;
}

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <GLUT/glut.h>
using namespace std;

void reshape(int width, int height);
void display();
void renderScene();
void mouseMove(int x, int y);
void mouseClick(int button, int state, int x, int y);
void keyboardPressed(unsigned char key, int x, int y);

class STATE {
  private:
    bool kontrola;
    bool odsijecanje;
  public:
    STATE() {kontrola = odsijecanje = false;}

    bool getKontrola() {return kontrola;}
    bool getOdsijecanje() {return odsijecanje;}

    void altKontrola() {kontrola = !kontrola;}
    void altOdsijecanje() {odsijecanje = !odsijecanje;}

};

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Zadatak 3.");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboardPressed);
  glutMouseFunc(mouseClick);
  glutPassiveMotionFunc(mouseMove);

  glutMainLoop();
  return 0;
}

void display() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  renderScene();

  glutSwapBuffers();
}

void reshape(int width, int height) {
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width-1, height-1, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
  glPointSize(1.0f);
  // moji segmenti crno
  // tocni segmenti crveno
}

void keyboardPressed(unsigned char key, int x, int y) {
  glutPostRedisplay();
}

void mouseClick(int button, int status, int x, int y) {
  glutPostRedisplay();
}

void mouseMove(int x, int y) {
  glutPostRedisplay();
}

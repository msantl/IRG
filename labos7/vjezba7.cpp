#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <err.h>
#include <GLUT/glut.h>

#include <fstream>
#include <float.h>
#include "mojlib.h"

using namespace std;

class point_t {
  private:
    double x, y, z, h;

  public:
    point_t() {}

    point_t(double _x, double _y, double _z) : x(_x), y(_y), z(_z), h(1.) {}

    point_t(double _x, double _y, double _z, double _h) : x(_x), y(_y), z(_z), h(_h) {}

    double getX() {return x;}
    double getY() {return y;}
    double getZ() {return z;}
    double getH() {return h;}

    void setX(double _x) {x = _x;}
    void setY(double _y) {y = _y;}
    void setZ(double _z) {z = _z;}
    void setH(double _h) {h = _h;}
};

vector< point_t > vrh;

void reshape(int width, int height);
void display();
void renderScene();
void normalize(vector< point_t > &v);

int main(int argc, char **argv) {
  /*
   * ucitavanje tocaka
   */
  string buff;
  vector< string > s_buff;

  ifstream tocke;
  tocke.open(argv[1]);

  while (getline(tocke, buff)) {
    s_buff = split(buff, " ");

    double x, y, z;
    sscanf(s_buff[0].c_str(), "%lf", &x);
    sscanf(s_buff[1].c_str(), "%lf", &y);
    sscanf(s_buff[2].c_str(), "%lf", &z);

    point_t T = point_t(x, y, z);

    vrh.push_back(T);
  }

  tocke.close();

  normalize(vrh);

  printf("===============================\n");
  printf("Ucitane tocke iz %s\n", argv[1]);
  printf("===============================\n");
  for (
    vector< point_t >::iterator
    it = vrh.begin();
    it != vrh.end();
    ++it
  ) {
    printf("%.2lf %.2lf %.2lf\n", it->getX(), it->getY(), it->getZ());
  }

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
}

void renderScene() {
  glPointSize(1.0f);
  glColor3f(0.0f, 0.0f, 0.0f);

  /*
   * crtanje zadanih tocaka povezanih linijama
   */
  glBegin(GL_LINE_STRIP);
  for (
    vector< point_t >::iterator
    it = vrh.begin();
    it != vrh.end();
    ++it
  ) {
    glVertex3f(it->getX(), it->getY(), it->getZ());
  }
  glEnd();

  vector< point_t > aprox;

  int N = vrh.size() - 1;
  for (double t = 0; t <= 1; t += 0.01) {
    point_t T = point_t(0, 0, 0);
    int i = 0;
    for (
      vector< point_t >::iterator
      it = vrh.begin();
      it != vrh.end();
      ++it
    ) {
      T.setX(T.getX() + it->getX() * nCr(N, i) * pow(t, i) * pow(1 - t, N - i));
      T.setY(T.getY() + it->getY() * nCr(N, i) * pow(t, i) * pow(1 - t, N - i));
      T.setZ(T.getZ() + it->getZ() * nCr(N, i) * pow(t, i) * pow(1 - t, N - i));
      i += 1;
    }
    aprox.push_back(T);
  }


  glColor3f(1.0f, 0.0f, 0.0f);
  /*
   * ctanje bezierove krivulje zadanih bernsteinovim polinomima
   */
  glBegin(GL_LINE_STRIP);
  for (
    vector< point_t >::iterator
    it = aprox.begin();
    it != aprox.end();
    ++it
  ) {
    glVertex3f(it->getX(), it->getY(), it->getZ());
  }
  glEnd();

  return;
}

void normalize(vector< point_t > &v) {
  /*
   * Vrijednosti potrebne za rad
   */
  double xmax = DBL_MIN, xmin = DBL_MAX;
  double ymax = DBL_MIN, ymin = DBL_MAX;
  double zmax = DBL_MIN, zmin = DBL_MAX;
  for (
    vector< point_t >::iterator
    it = vrh.begin();
    it != vrh.end();
    ++it
  ) {
    xmax = max(xmax, it->getX());
    xmin = min(xmin, it->getX());
    ymax = max(ymax, it->getY());
    ymin = min(ymin, it->getY());
    zmax = max(zmax, it->getZ());
    zmin = min(zmin, it->getZ());
  }


  double xsr = (xmin + xmax) / 2.;
  double ysr = (ymin + ymax) / 2.;
  double zsr = (zmin + zmax) / 2.;

  double M = max(max(xmax - xmin, ymax - ymin), zmax - zmin);

  // normalizacija
  // translacija
  for (
      vector< point_t >::iterator
      it = vrh.begin();
      it != vrh.end();
      ++it
  ) {
    it->setX(it->getX() - xsr);
    it->setY(it->getY() - ysr);
    it->setZ(it->getZ() - zsr);
  }
  // skaliranje
  for (
      vector< point_t >::iterator
      it = vrh.begin();
      it != vrh.end();
      ++it
  ) {
    it->setX(2 * it->getX() / M);
    it->setY(2 * it->getY() / M);
    it->setZ(2 * it->getZ() / M);
  }

  return;
}

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <err.h>
#include <GLUT/glut.h>

#include <float.h>
#include <fstream>
#include "mojlib.h"
using namespace std;

class point_t {
  private:
    double x, y, z, h;

  public:
    point_t() {}

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

class poly_t {
  private:
    int p1, p2, p3;
    double A, B, C, D;

  public:
    poly_t() {}

    poly_t(int _p1, int _p2, int _p3) : p1(_p1), p2(_p2), p3(_p3) {}

    int getP1() {return p1 - 1;}
    int getP2() {return p2 - 1;}
    int getP3() {return p3 - 1;}

    void setA(double _A) {A = _A;}
    void setB(double _B) {B = _B;}
    void setC(double _C) {C = _C;}
    void setD(double _D) {D = _D;}

    double getA() {return A;}
    double getB() {return B;}
    double getC() {return C;}
    double getD() {return D;}
};

/*
 * stanje sustava
 */
double t;
bool ukloni_straznje;
point_t O, G(0, 0, 0, 1.);

/*
 * Matrice transformacije
 */
double T1[4][4], T2[4][4], T3[4][4], T4[4][4], T5[4][4];
/*
 * Matrica projekcije
 */
double P1[4][4];
double mTrans[4][4];

/*
 * Strukture podataka
 */
vector< point_t > vrh;
vector< poly_t > poligon;
vector< point_t > putanja;

/*
 * Deklaracije funkcija
 */
void multiply(double a[4][4], double b[4][4], double c[4][4]);
point_t transform(point_t t, double m[4][4]);
void reshape(int width, int height);
void display();
void renderScene();
void izracunajNormalu(poly_t &a);
void normalize(vector< point_t > &v);

vector< point_t > izracunajProjekciju(void);

  void keyboardPressed(unsigned char key, int x, int y);

int main(int argc, char **argv) {
  ukloni_straznje = 1;
  t = 0.5;
  /*
   * Parsiranje datoteke
   */
  ifstream objekt, postavke;
  string buff;
  vector< string > s_buff;

  objekt.open(argv[1], fstream::in);
  while (getline(objekt, buff)) {
    s_buff = split(buff, " ");

    if (s_buff[0] == "#") {
      // komentar
    } else if (s_buff[0] == "g") {
      // grupa
    } else if (s_buff[0] == "v") {
      // vrh
      double x, y, z;
      sscanf(s_buff[1].c_str(), "%lf", &x);
      sscanf(s_buff[2].c_str(), "%lf", &y);
      sscanf(s_buff[3].c_str(), "%lf", &z);

      point_t T = point_t(x, y, z, 1.);
      vrh.push_back(T);

    } else if (s_buff[0] == "f") {
      //face
      int p1, p2, p3;
      sscanf(s_buff[1].c_str(), "%d", &p1);
      sscanf(s_buff[2].c_str(), "%d", &p2);
      sscanf(s_buff[3].c_str(), "%d", &p3);

      poly_t P = poly_t(p1, p2, p3);

      izracunajNormalu(P);

      poligon.push_back(P);
    }
  }

  objekt.close();

  ifstream tocke;
  tocke.open(argv[2]);

  while (getline(tocke, buff)) {
    s_buff = split(buff, " ");

    double x, y, z;
    sscanf(s_buff[0].c_str(), "%lf", &x);
    sscanf(s_buff[1].c_str(), "%lf", &y);
    sscanf(s_buff[2].c_str(), "%lf", &z);

    point_t T = point_t(x, y, z, 1.);

    putanja.push_back(T);
  }

  tocke.close();

  /*
   * sanity print
   */
  printf("Ucitano tijelo iz %s\n", argv[1]);
  printf("=====================================\n");
  printf("Vrhova: %d, Poligona: %d\n", (int)vrh.size(), (int)poligon.size());
  printf("=====================================\n");
  printf("Ucitana putanja iz %s\n", argv[2]);
  printf("=====================================\n");
  printf("Tocaka na putanji: %d\n", (int)putanja.size());
  printf("=====================================\n");
  for (
    vector< point_t >::iterator
    it = putanja.begin();
    it != putanja.end();
    ++it
  ) {
    printf("%.3lf %.3lf %.3lf\n", it->getX(), it->getY(), it->getZ());
  }
  printf("=====================================\n");

  /*
   * Inicijalizacija GLUT-a
   */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(640, 480);

  glutInitWindowPosition(50, 50);
  glutCreateWindow("Zadatak 7. drugi dio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboardPressed);

  glutMainLoop();
  return 0;
}

void display() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

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

  vector< point_t > projekcija = izracunajProjekciju();

  for (
    vector< poly_t >::iterator
    it = poligon.begin();
    it != poligon.end();
    ++it
  ) {
    // provjera kuta normale i glediste -> ociste
    double r =
      it->getA() * (O.getX()) +
      it->getB() * (O.getY()) +
      it->getC() * (O.getZ()) +
      it->getD();

    if (ukloni_straznje == 0 || r > 0) {
      glBegin(GL_LINE_LOOP);
        glVertex2f(projekcija[it->getP1()].getX(), projekcija[it->getP1()].getY());
        glVertex2f(projekcija[it->getP2()].getX(), projekcija[it->getP2()].getY());
        glVertex2f(projekcija[it->getP3()].getX(), projekcija[it->getP3()].getY());
      glEnd();
    }
  }
  return;
}

void multiply(double a[4][4], double b[4][4], double c[4][4]) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      double temp = 0;
      for (int k = 0; k < 4; ++k) {
        temp += a[i][k] * b[k][j];
      }
      c[i][j] = temp;
    }
  }
  return;
}

point_t transform(point_t t, double M[4][4]) {
  point_t result;

  result.setX(
    t.getX() * M[0][0] + t.getY() * M[1][0] + t.getZ() * M[2][0] + t.getH() * M[3][0]
  );

  result.setY(
    t.getX() * M[0][1] + t.getY() * M[1][1] + t.getZ() * M[2][1] + t.getH() * M[3][1]
  );

  result.setZ(
    t.getX() * M[0][2] + t.getY() * M[1][2] + t.getZ() * M[2][2] + t.getH() * M[3][2]
  );

  result.setH(
    t.getX() * M[0][3] + t.getY() * M[1][3] + t.getZ() * M[2][3] + t.getH() * M[3][3]
  );

  /*
   * Vracanje u radni prostor
   */
  if (result.getH() != 1.0 && result.getH() != 0) {
    result.setX(result.getX() / result.getH());
    result.setY(result.getY() / result.getH());
    result.setZ(result.getZ() / result.getH());

    result.setH(1.0);
  }

  return result;
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
    it = v.begin();
    it != v.end();
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
      it = v.begin();
      it != v.end();
      ++it
  ) {
    it->setX(it->getX() - xsr);
    it->setY(it->getY() - ysr);
    it->setZ(it->getZ() - zsr);
  }
  // skaliranje
  for (
      vector< point_t >::iterator
      it = v.begin();
      it != v.end();
      ++it
  ) {
    it->setX(2 * it->getX() / M);
    it->setY(2 * it->getY() / M);
    it->setZ(2 * it->getZ() / M);
  }

  return;
}

void izracunajNormalu(poly_t &a) {
  double v2x = vrh[a.getP2()].getX() - vrh[a.getP1()].getX();
  double v2y = vrh[a.getP2()].getY() - vrh[a.getP1()].getY();
  double v2z = vrh[a.getP2()].getZ() - vrh[a.getP1()].getZ();

  double v3x = vrh[a.getP3()].getX() - vrh[a.getP1()].getX();
  double v3y = vrh[a.getP3()].getY() - vrh[a.getP1()].getY();
  double v3z = vrh[a.getP3()].getZ() - vrh[a.getP1()].getZ();

  a.setA(v2y * v3z - v3y * v2z);
  a.setB(v3x * v2z - v2x * v3z);
  a.setC(v2x * v3y - v3x * v2y);

  a.setD(
    -vrh[a.getP1()].getX() * a.getA() +
    -vrh[a.getP1()].getY() * a.getB() +
    -vrh[a.getP1()].getZ() * a.getC()
  );

  return;
}

void keyboardPressed(unsigned char key, int x, int y) {
  bool refresh = false;

  switch(key) {
    case 'p':
      ukloni_straznje = 1 - ukloni_straznje;
      refresh = true;
      break;
    case 'j':
      t += 0.01;
      t = min(t, 1.);
      refresh = true;
      break;
    case 'k':
      t -= 0.01;
      t = max(t, 0.);
      refresh = true;
      break;
    default:
      refresh = false;
      break;
  }

  if (refresh) {
    glutPostRedisplay();
  }
}

vector< point_t > izracunajProjekciju(void) {
  vector< point_t > projekcija = vrh;
  /*
   * izracunaj tocku ocista za dani parametar t
   */
  O = point_t(0, 0, 0, 1.);
  int N = putanja.size() - 1;

  int i = 0;
  for (
    vector< point_t >::iterator
    it = putanja.begin();
    it != putanja.end();
    ++it
  ) {
    O.setX(O.getX() + it->getX() * nCr(N, i) * pow(t, i) * pow(1 - t, N - i));
    O.setY(O.getY() + it->getY() * nCr(N, i) * pow(t, i) * pow(1 - t, N - i));
    O.setZ(O.getZ() + it->getZ() * nCr(N, i) * pow(t, i) * pow(1 - t, N - i));

    i += 1;
  }

  /*
   * Inicijalizacija matrica transformacija
   */
  point_t G1, G2, G3;
  // T1 - pomak ishodista u tocku O
  T1[0][0] = 1;         T1[0][1] = 0;         T1[0][2] = 0;         T1[0][3] = 0;
  T1[1][0] = 0;         T1[1][1] = 1;         T1[1][2] = 0;         T1[1][3] = 0;
  T1[2][0] = 0;         T1[2][1] = 0;         T1[2][2] = 1;         T1[2][3] = 0;
  T1[3][0] = -O.getX(); T1[3][1] = -O.getY(); T1[3][2] = -O.getZ(); T1[3][3] = 1;

  G1 = point_t(G.getX() - O.getX(), G.getY() - O.getY(), G.getZ() - O.getZ(), 1.);

  // T2 rotacija za kut alfa oko z osi
  double cosL, sinL;
  if (G1.getX() == 0 && G1.getY() == 0) {
    cosL = 1;
    sinL = 0;

    G2 = point_t(G1.getX(), G1.getY(), G1.getZ(), G1.getH());
  } else {
    cosL = G1.getX() / sqrt(pow(G1.getX(), 2) + pow(G1.getY(), 2));
    sinL = G1.getY() / sqrt(pow(G1.getX(), 2) + pow(G1.getY(), 2));

    G2 = point_t(sqrt(pow(G1.getX(), 2) + pow(G1.getY(), 2)), 0, G1.getZ(), 1.);
  }

  T2[0][0] = cosL; T2[0][1] = -sinL; T2[0][2] = 0; T2[0][3] = 0;
  T2[1][0] = sinL; T2[1][1] = cosL;  T2[1][2] = 0; T2[1][3] = 0;
  T2[2][0] = 0;    T2[2][1] = 0;     T2[2][2] = 1; T2[2][3] = 0;
  T2[3][0] = 0;    T2[3][1] = 0;     T2[3][2] = 0; T2[3][3] = 1;

  // T3 rotacija za kut beta oko y osi
  double cosB, sinB;
  if (G2.getX() == 0 && G2.getZ() == 0) {
    cosB = 1;
    sinB = 0;

    G3 = point_t(G2.getX(), G2.getY(), G2.getZ(), G2.getH());
  } else {
    cosB = G2.getZ() / sqrt(pow(G2.getX(), 2) + pow(G2.getZ(), 2));
    sinB = G2.getX() / sqrt(pow(G2.getX(), 2) + pow(G2.getZ(), 2));

    G3 = point_t(0, 0, sqrt(pow(G2.getX(), 2) + pow(G2.getZ(), 2)), 1.);
  }
  T3[0][0] = cosB;  T3[0][1] = 0; T3[0][2] = sinB; T3[0][3] = 0;
  T3[1][0] = 0;     T3[1][1] = 1; T3[1][2] = 0;    T3[1][3] = 0;
  T3[2][0] = -sinB; T3[2][1] = 0; T3[2][2] = cosB; T3[2][3] = 0;
  T3[3][0] = 0;     T3[3][1] = 0; T3[3][2] = 0;    T3[3][3] = 1;

  // T4 rotacija za kut 90 oko z osi
  T4[0][0] = 0; T4[0][1] = -1; T4[0][2] = 0; T4[0][3] = 0;
  T4[1][0] = 1; T4[1][1] = 0;  T4[1][2] = 0; T4[1][3] = 0;
  T4[2][0] = 0; T4[2][1] = 0;  T4[2][2] = 1; T4[2][3] = 0;
  T4[3][0] = 0; T4[3][1] = 0;  T4[3][2] = 0; T4[3][3] = 1;

  // T5 promjena predznaka na x osi
  T5[0][0] = -1; T5[0][1] = 0; T5[0][2] = 0; T5[0][3] = 0;
  T5[1][0] = 0;  T5[1][1] = 1; T5[1][2] = 0; T5[1][3] = 0;
  T5[2][0] = 0;  T5[2][1] = 0; T5[2][2] = 1; T5[2][3] = 0;
  T5[3][0] = 0;  T5[3][1] = 0; T5[3][2] = 0; T5[3][3] = 1;

  // matrica transformacije
  double tempM1[4][4], tempM2[4][4], tempM3[4][4];
  multiply(T1, T2, tempM1);
  multiply(tempM1, T3, tempM2);
  multiply(tempM2, T4, tempM3);
  multiply(tempM3, T5, mTrans);

  // matrica projekcije
  P1[0][0] = 1; P1[0][1] = 0; P1[0][2] = 0; P1[0][3] = 0;
  P1[1][0] = 0; P1[1][1] = 1; P1[1][2] = 0; P1[1][3] = 0;
  P1[2][0] = 0; P1[2][1] = 0; P1[2][2] = 0; P1[2][3] = 1. / G3.getZ();
  P1[3][0] = 0; P1[3][1] = 0; P1[3][2] = 0; P1[3][3] = 0;

  // izracun novih koordinata vrhova
  for (
    vector< point_t >::iterator
    it = projekcija.begin();
    it != projekcija.end();
    ++it
  ) {
    (*it) = transform(*it, mTrans);
    (*it) = transform(*it, P1);
  }

  // normalizacija
  normalize(projekcija);

  printf("=====================================\n");
  printf("Ociste: %.3lf %.3lf %.3lf\n", O.getX(), O.getY(), O.getZ());
  printf("Glediste: %.3lf %.3lf %.3lf\n", G.getX(), G.getY(), G.getZ());
  printf("=====================================\n");
  // ispis matrica
  printf("Matrice transformacije pogleda\n");
  printf("=====================================\n");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%.3lf ", mTrans[i][j]);
    }
    printf("\n");
  }
  printf("=====================================\n");
  printf("Matrice perspektivne projekcije\n");
  printf("=====================================\n");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%.3lf ", P1[i][j]);
    }
    printf("\n");
  }

  return projekcija;
}

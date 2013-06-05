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
    double A, B, C;
    int n;

  public:
    point_t() {}

    point_t(double _x, double _y, double _z) : x(_x), y(_y), z(_z), h(1.0) {
      A = 0;
      B = 0;
      C = 0;
      n = 0;
    }

    point_t(double _x, double _y, double _z, double _h) : x(_x), y(_y), z(_z), h(_h) {
      A = 0;
      B = 0;
      C = 0;
      n = 0;
    }

    double getX() {return x;}
    double getY() {return y;}
    double getZ() {return z;}
    double getH() {return h;}

    double getA() {return A / n;}
    double getB() {return B / n;}
    double getC() {return C / n;}

    void setX(double _x) {x = _x;}
    void setY(double _y) {y = _y;}
    void setZ(double _z) {z = _z;}
    void setH(double _h) {h = _h;}

    void append(double a, double b, double c) {
      A += a;
      B += b;
      C += c;
      n += 1;
    }
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
int width, height;
int vrsta_prikaza;
double pocetni_x, pocetni_y;
point_t ociste, izvor;

/*
 * Strukture podataka
 */
vector< point_t > vrh;
vector< poly_t > poligon;

/*
 * Deklaracije funkcija
 */
void reshape(int width, int height);
void display();
void renderScene();
void updatePerspective();

void izracunajNormalu(poly_t &a);
point_t izracunajSrediste(poly_t p);
double izracunajIntenzitet(point_t no, point_t iz);

void normalize(vector< point_t > &v);
void normalizeVector(point_t &v);

void keyboardPressed(unsigned char key, int x, int y);
void mousePressed(int button, int state, int x, int y);

void zicaniPrikaz();
void konstantnoSjencanjePrikaz();
void gouraudovoSjencanjePrikaz();

int main(int argc, char **argv) {
  /*
   * Parsiranje datoteke
   */
  ifstream objekt;
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

      poligon.push_back(P);
    }
  }

  /*
   * ucitaj ociste
   */
  {
    double x, y, z;
    printf("Unesite polozaj ocista: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    ociste = point_t(x, y, z);

    pocetni_x = x; pocetni_y = y;
  }
  /*
   * ucitaj izvor
   */
  {
    double x, y, z;
    printf("Unesite polozaj izvora: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    izvor = point_t(x, y, z);
  }

  objekt.close();
  /*
   * normaliziraj ucitani poligon
   */
  normalize(vrh);

  /*
   * izracunaj normale poligona
   */
  for (
    vector< poly_t >::iterator
    it = poligon.begin();
    it != poligon.end();
    ++it
  ) {
    izracunajNormalu(*it);
  }

  /*
   * izracunaj normale vrhova
   */
  for (
    vector< poly_t >::iterator
    it = poligon.begin();
    it != poligon.end();
    ++it
  ) {
    vrh[it->getP1()].append(it->getA(), it->getB(), it->getC());
    vrh[it->getP2()].append(it->getA(), it->getB(), it->getC());
    vrh[it->getP3()].append(it->getA(), it->getB(), it->getC());
  }

  /*
   * sanity print
   */
  printf("Ucitano tijelo iz %s\n", argv[1]);
  printf("=====================================\n");
  printf("Vrhova: %d, Poligona: %d\n", (int)vrh.size(), (int)poligon.size());
  printf("=====================================\n");

  /*
   * Inicijalizacija GLUT-a
   */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);

  glutInitWindowPosition(50, 50);
  glutCreateWindow("Zadatak 7.");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboardPressed);
  glutMouseFunc(mousePressed);

  glutMainLoop();
  return 0;
}

void display() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  // crtaj scenu
  renderScene();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  width = w; height = h;
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	gluPerspective(45.0, (float)width/height, 0.5, 8.0);
	gluLookAt (ociste.getX(), ociste.getY(), ociste.getZ(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode (GL_MODELVIEW);
}

void zicaniPrikaz() {
  /*
   * Zicani prikaz uz uklanjanje straznjih poligona
   */
  for (
      vector< poly_t >::iterator
      it = poligon.begin();
      it != poligon.end();
      ++it
      ) {
    /*
     * ukloni straznje poligone
     */
    point_t sr = izracunajSrediste(*it);
    double cosL =
      it->getA() * (ociste.getX() - sr.getX()) +
      it->getB() * (ociste.getY() - sr.getY()) +
      it->getC() * (ociste.getZ() - sr.getZ());

    if (cosL < 0) continue;

    glBegin(GL_LINE_LOOP);
    glVertex3f(vrh[it->getP1()].getX(), vrh[it->getP1()].getY(), vrh[it->getP1()].getZ());
    glVertex3f(vrh[it->getP2()].getX(), vrh[it->getP2()].getY(), vrh[it->getP2()].getZ());
    glVertex3f(vrh[it->getP3()].getX(), vrh[it->getP3()].getY(), vrh[it->getP3()].getZ());
    glEnd();
  }

  return;
}

void konstantnoSjencanjePrikaz() {
  /*
   * Konstantno sjencanje uz uklanjanje straznjih poligona
   */
  for (
    vector< poly_t >::iterator
    it = poligon.begin();
    it != poligon.end();
    ++it
  ) {
    /*
     * ukloni straznje poligone
     */
    point_t sr = izracunajSrediste(*it);
    double cosL =
      it->getA() * (ociste.getX() - sr.getX()) +
      it->getB() * (ociste.getY() - sr.getY()) +
      it->getC() * (ociste.getZ() - sr.getZ());

    if (cosL < 0) continue;

    /*
     * sjencanje
     */
    point_t iz =
      point_t(izvor.getX() - sr.getX(), izvor.getY() - sr.getY(), izvor.getZ() - sr.getZ());
    point_t no = point_t(it->getA(), it->getB(), it->getC());

    double I = izracunajIntenzitet(no, iz);

    glBegin(GL_TRIANGLES);
    glColor3ub(I, I/2, I);
    glVertex3f(vrh[it->getP1()].getX(), vrh[it->getP1()].getY(), vrh[it->getP1()].getZ());
    glColor3ub(I, I/2, I);
    glVertex3f(vrh[it->getP2()].getX(), vrh[it->getP2()].getY(), vrh[it->getP2()].getZ());
    glColor3ub(I, I/2, I);
    glVertex3f(vrh[it->getP3()].getX(), vrh[it->getP3()].getY(), vrh[it->getP3()].getZ());
    glEnd();
  }

  return;
}

void gouraudovoSjencanjePrikaz() {
  /*
   * Gouraudovo sjencanje  uz uklanjanje straznjih poligona
   */
  for (
    vector< poly_t >::iterator
    it = poligon.begin();
    it != poligon.end();
    ++it
  ) {
    /*
     * ukloni straznje poligone
     */
    point_t sr = izracunajSrediste(*it);
    double cosL =
      it->getA() * (ociste.getX() - sr.getX()) +
      it->getB() * (ociste.getY() - sr.getY()) +
      it->getC() * (ociste.getZ() - sr.getZ());

    if (cosL < 0) continue;

    point_t iz =
      point_t(izvor.getX() - sr.getX(), izvor.getY() - sr.getY(), izvor.getZ() - sr.getZ());
    point_t no1 = point_t(vrh[it->getP1()].getA(), vrh[it->getP1()].getB(), vrh[it->getP1()].getC());
    point_t no2 = point_t(vrh[it->getP2()].getA(), vrh[it->getP2()].getB(), vrh[it->getP2()].getC());
    point_t no3 = point_t(vrh[it->getP3()].getA(), vrh[it->getP3()].getB(), vrh[it->getP3()].getC());

    double I1 = izracunajIntenzitet(no1, iz);
    double I2 = izracunajIntenzitet(no2, iz);
    double I3 = izracunajIntenzitet(no3, iz);

    glBegin(GL_TRIANGLES);
    glColor3ub(I1, I1/2, I1);
    glVertex3f(vrh[it->getP1()].getX(), vrh[it->getP1()].getY(), vrh[it->getP1()].getZ());
    glColor3ub(I2, I2/2, I2);
    glVertex3f(vrh[it->getP2()].getX(), vrh[it->getP2()].getY(), vrh[it->getP2()].getZ());
    glColor3ub(I3, I3/2, I3);
    glVertex3f(vrh[it->getP3()].getX(), vrh[it->getP3()].getY(), vrh[it->getP3()].getZ());
    glEnd();
  }

  return;
}

void renderScene() {
  glPointSize(1.0f);
  glColor3f(1.0f, 0.0f, 0.0f);

  if (vrsta_prikaza == 0) {
    zicaniPrikaz();
  } else if (vrsta_prikaza == 1) {
    konstantnoSjencanjePrikaz();
  } else if (vrsta_prikaza == 2) {
    gouraudovoSjencanjePrikaz();
  }

  return;
}

void updatePerspective() {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45.0, (float)width/height, 0.5, 8.0);
	gluLookAt (ociste.getX(), ociste.getY(), ociste.getZ(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode (GL_MODELVIEW);
}

point_t izracunajSrediste(poly_t p) {
  double x_sr = vrh[p.getP1()].getX() + vrh[p.getP2()].getX() + vrh[p.getP3()].getX();
  double y_sr = vrh[p.getP1()].getY() + vrh[p.getP2()].getY() + vrh[p.getP3()].getY();
  double z_sr = vrh[p.getP1()].getZ() + vrh[p.getP2()].getZ() + vrh[p.getP3()].getZ();

  x_sr /= 3.;
  y_sr /= 3.;
  z_sr /= 3.;

  return point_t(x_sr, y_sr, z_sr);
}

void normalizeVector(point_t &v) {
  double absolute = sqrt(
    v.getX() * v.getX() +
    v.getY() * v.getY() +
    v.getZ() * v.getZ());

  v.setX(v.getX() / absolute);
  v.setY(v.getY() / absolute);
  v.setZ(v.getZ() / absolute);
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

double izracunajIntenzitet(point_t no, point_t iz) {
  // ambijentalna komponenta
  double Ia = 100;
  double ka = 0.5;

  // difuzna komponenta
  double Ii = 200;
  double kd = 0.8;

  // normalizacija vektora
  normalizeVector(no);
  normalizeVector(iz);

  // izracun koeficijenata za sjecanje
  double LN =
    iz.getX() * no.getX() +
    iz.getY() * no.getY() +
    iz.getZ() * no.getZ();

  if (LN < 0) {
    LN = 0;
  }

  return Ii * kd * LN + Ia * ka;
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

void mousePressed(int button, int state, int x, int y) {
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    ociste.setX(pocetni_x);
    ociste.setY(pocetni_y);
  }

  updatePerspective();
  glutPostRedisplay();
}

void keyboardPressed(unsigned char key, int x, int y) {
  switch (key) {
    case 'l':
      ociste.setX(ociste.getX() + 0.1);
      break;
    case 'j':
      ociste.setY(ociste.getY() + 0.1);
      break;
    case 'k':
      ociste.setY(ociste.getY() - 0.1);
      break;
    case 'h':
      ociste.setX(ociste.getX() - 0.1);
      break;
    case 'r':
      ociste.setX(pocetni_x);
      ociste.setY(pocetni_y);
      break;
    case 'p':
      vrsta_prikaza = (vrsta_prikaza + 1) % 3;
      break;
    default:
      break;
  }

  updatePerspective();
  glutPostRedisplay();
}

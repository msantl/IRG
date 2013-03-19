#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <GLUT/glut.h>
using namespace std;

#define EPS 1e-9

// offset za kontrolnu liniju
#define OFFSET 4

// cohen sutherland konstante
#define MAXY 8
#define MINY 4
#define MAXX 2
#define MINX 1

void reshape(int width, int height);
void display();
void renderScene();
void mouseClick(int button, int state, int x, int y);
void keyboardPressed(unsigned char key, int x, int y);

class point_t {
  private:
    int x, y;
  public:
    point_t() {}
    point_t(int _x, int _y) : x(_x), y(_y) {}

    int getX() {return x;}
    int getY() {return y;}

    void setX(int _x) {x = _x;}
    void setY(int _y) {y = _y;}
};

class segment_t {
  private:
    point_t *a, *b;
  public:
    segment_t() : a(NULL), b(NULL){}
    segment_t(point_t *_a, point_t *_b) : a(_a), b(_b) {}

    // copy constructor
    segment_t(segment_t *s) {
      a = new point_t(*(s->getStart()));
      b = new point_t(*(s->getEnd()));
    }

    ~segment_t() {
      delete a;
      delete b;
    }

    point_t* getStart() {return a;}
    point_t* getEnd() {return b;}

    void setStart(point_t *_a) {a = _a;}
    void setEnd(point_t *_b) {b = _b;}

    bool isDefined() {
      return (a != NULL && b != NULL);
    }
};

class state_t {
  private:
    bool kontrola;
    bool odsijecanje;

    segment_t* aktivan;
    vector< segment_t* > segmenti;

    int width, height;

  public:
    state_t() {kontrola = odsijecanje = false; aktivan = NULL;}

    ~state_t() {
      delete aktivan;
      for (vector< segment_t* >::iterator it = segmenti.begin(); it != segmenti.end(); ++it) {
        delete (*it);
      }
    }

    void setWidth(int w) {width = w;}
    void setHeight(int h) {height = h;}

    int getWidth() {return width;}
    int getHeight() {return height;}

    int getMaxX() {
      if (odsijecanje) {
        return 3 * width / 4;
      }
      return width;
    }
    int getMinX() {
      if (odsijecanje){
        return 1 * width / 4;
      }
      return 0;
    }
    int getMaxY() {
      if (odsijecanje) {
        return 3 * height / 4;
      }
      return height;
    }
    int getMinY() {
      if (odsijecanje) {
        return 1 * height / 4;
      }
      return 0;
    }

    bool getKontrola() {return kontrola;}
    bool getOdsijecanje() {return odsijecanje;}

    void altKontrola() {kontrola = !kontrola;}
    void altOdsijecanje() {odsijecanje = !odsijecanje;}

    segment_t* getAktivan() {
      if (aktivan == NULL || aktivan->isDefined()) {
        aktivan = new segment_t();
      }

      return aktivan;
    }

    vector< segment_t* > getSegmenti() {
      return segmenti;
    }

    void addSegment(segment_t *s) {
      segmenti.push_back(s);
    }

} STATE;


void myBresenham(point_t *a, point_t *b);
int getMask(double x, double y);
bool myCohenSutherland(segment_t *s);

int main(int argc, char **argv) {
  STATE.setWidth(500);
  STATE.setHeight(500);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(STATE.getWidth(), STATE.getHeight());

  glutInitWindowPosition(0, 0);
  glutCreateWindow("Zadatak 3.");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboardPressed);
  glutMouseFunc(mouseClick);

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
  STATE.setWidth(width);
  STATE.setHeight(height);

  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width-1, height-1, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
  glPointSize(1.0f);
  vector< segment_t* > segmenti = STATE.getSegmenti();
  // moji segmenti crno
  if (STATE.getOdsijecanje() == true) {
    // nacrtaj zeleni kvadrat
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);

    glVertex2i(1 * STATE.getWidth() / 4, 1 * STATE.getHeight() / 4);
    glVertex2i(3 * STATE.getWidth() / 4, 1 * STATE.getHeight() / 4);
    glVertex2i(3 * STATE.getWidth() / 4, 3 * STATE.getHeight() / 4);
    glVertex2i(1 * STATE.getWidth() / 4, 3 * STATE.getHeight() / 4);
    glVertex2i(1 * STATE.getWidth() / 4, 1 * STATE.getHeight() / 4);

    glEnd();
  }

  glColor3f(0.0f, 0.0f, 0.0f);
  for (vector< segment_t* >::iterator it = segmenti.begin(); it != segmenti.end(); ++it) {
    bool crtaj = true;
    // kopiraj segment zbog odsijecanja
    segment_t* seg = new segment_t(*it);
    // nacrtaj vidljivi dio segmenta
    if (STATE.getOdsijecanje() == true) {
      crtaj = myCohenSutherland(seg);
    }

    if (crtaj) {
      myBresenham(seg->getStart(), seg->getEnd());
    }

    // obrisi segment
    delete seg;
  }

  // tocni segmenti crveno
  if (STATE.getKontrola() == true) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for (vector< segment_t* >::iterator it = segmenti.begin(); it != segmenti.end(); ++it) {
      glVertex2i((*it)->getStart()->getX(), (*it)->getStart()->getY() + OFFSET);
      glVertex2i((*it)->getEnd()->getX(), (*it)->getEnd()->getY() + OFFSET);
    }
    glEnd();
  }
}

void keyboardPressed(unsigned char key, int x, int y) {
  switch(key) {
    case 'k':
      STATE.altKontrola();
      break;
    case 'o':
      STATE.altOdsijecanje();
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

void mouseClick(int button, int status, int x, int y) {
  // ako je pritisnut lijevi gumb misa
  if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
    segment_t* aktivan = STATE.getAktivan();
    point_t* point = new point_t(x, y);

    if (aktivan->getStart() == NULL) {
      aktivan->setStart(point);
    } else if (aktivan->getEnd() == NULL) {
      aktivan->setEnd(point);
    }

    if (aktivan->isDefined()) {
      STATE.addSegment(aktivan);
    }

    glutPostRedisplay();
  }
}

// kutevi od 0 do 90
void bresenham_nacrtaj_cjelobrojni2(int xs, int ys, int xe, int ye) {
  int x, yc, korekcija;
  int a, yf;

  if (ye - ys <= xe - xs) {
    a = 2*(ye - ys);
    yc = ys; yf = -(xe - xs); korekcija = -2*(xe - xs);
    for (x = xs; x <= xe; ++x) {
      glVertex2i(x, yc);
      yf = yf + a;
      if (yf >= 0) {
        yf = yf + korekcija;
        yc = yc + 1;
      }
    }
  } else {
    x = xe; xe = ye; ye = x;
    x = xs; xs = ys; ys = x;
    a = 2*(ye - ys);
    yc = ys; yf = -(xe - xs); korekcija = -2*(xe - xs);
    for (x = xs; x <= xe; ++x) {
      glVertex2i(yc, x);
      yf = yf + a;
      if (yf >= 0) {
        yf = yf + korekcija;
        yc = yc + 1;
      }
    }
  }
}


// kutevi od 0 do -90
void bresenham_nacrtaj_cjelobrojni3(int xs, int ys, int xe, int ye) {
  int x, yc, korekcija;
  int a, yf;
  if (-(ye-ys) <= xe - xs) {
    a = 2*(ye - ys);
    yc = ys;  yf = (xe - xs); korekcija = 2*(xe - xs);
    for (x = xs; x <= xe; ++x) {
      glVertex2i(x, yc);
      yf = yf + a;
      if (yf <= 0) {
        yf = yf + korekcija;
        yc = yc - 1;
      }
    }
  } else {
    x = xe; xe = ys; ys = x;
    x = xs; xs = ye; ye = x;
    a = 2*(ye - ys);
    yc = ys; yf = (xe - xs); korekcija = 2*(xe - xs);
    for (x = xs; x <= xe; ++x) {
      glVertex2i(yc, x);
      yf = yf + a;
      if (yf <= 0) {
        yf = yf + korekcija;
        yc = yc - 1;
      }
    }
  }
}

void bresenham_nacrtaj_cjelobrojni(int xs, int ys, int xe, int ye) {
  if (xs <= xe) {
    if (ys <= ye) {
      bresenham_nacrtaj_cjelobrojni2(xs, ys, xe, ye);
    } else {
      bresenham_nacrtaj_cjelobrojni3(xs, ys, xe, ye);
    }
  } else {
    if (ys >= ye) {
      bresenham_nacrtaj_cjelobrojni2(xe, ye, xs, ys);
    } else {
      bresenham_nacrtaj_cjelobrojni3(xe, ye, xs, ys);
    }
  }
}

void myBresenham(point_t *a, point_t *b) {
  glBegin(GL_POINTS);
  bresenham_nacrtaj_cjelobrojni(a->getX(), a->getY(), b->getX(), b->getY());
  glEnd();
  return;
}

bool myCohenSutherland(segment_t *s) {
  bool ret = false;

  double x1 = s->getStart()->getX();
  double y1 = s->getStart()->getY();

  double x2 = s->getEnd()->getX();
  double y2 = s->getEnd()->getY();

  while (1) {
    int maskStart = getMask(x1, y1);
    int maskEnd = getMask(x2, y2);

    if (maskStart == 0 && maskEnd == 0) {
      // potpuno vidljiva
      ret = true;
      break;
    } else if ((maskStart & maskEnd) != 0) {
      // nije vidljivo
      ret = false;
      break;
    } else {
      // odsijecanje

      int mask = maskStart ? maskStart : maskEnd;
      double x, y;

      // y - y1 = (y2 - y1) * (x - x1) / (x2 - x1)

      if (mask & MAXY) {
        x = x1 + (x2 - x1) * (STATE.getMaxY() - y1) / (y2 - y1);
        y = STATE.getMaxY();
      } else if (mask & MINY) {
        x = x1 + (x2 - x1) * (STATE.getMinY() - y1) / (y2 - y1);
        y = STATE.getMinY();
      } else if (mask & MAXX) {
        y = y1 + (y2 - y1) * (STATE.getMaxX() - x1) / (x2 - x1);
        x = STATE.getMaxX();
      } else if (mask & MINX) {
        y = y1 + (y2 - y1) * (STATE.getMinX() - x1) / (x2 - x1);
        x = STATE.getMinX();
      }

      // update the coordinates
      if (mask == maskStart) {
        x1 = x;
        y1 = y;
      } else {
        x2 = x;
        y2 = y;
      }

    }
  }

  // update the coordinates
  s->getStart()->setX((int)x1);
  s->getStart()->setY((int)y1);
  s->getEnd()->setX((int)x2);
  s->getEnd()->setY((int)y2);

  return ret;
}

int getMask(double x, double y) {
  int mask = 0;

  if (y > STATE.getMaxY()) mask |= MAXY;
  if (y < STATE.getMinY()) mask |= MINY;
  if (x > STATE.getMaxX()) mask |= MAXX;
  if (x < STATE.getMinX()) mask |= MINX;

  return mask;
}



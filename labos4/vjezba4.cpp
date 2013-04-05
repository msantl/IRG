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
void mouseClick(int button, int state, int x, int y);
void mouseMoved(int x, int y);
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
} *kursor;

class edge_t {
  private:
    int a, b, c;
  public:
    edge_t() {}
    edge_t(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {}

    edge_t(point_t *s, point_t *e) {
      a = s->getY() - e->getY();
      b = e->getX() - s->getX();
      c = s->getX() * e->getY() - s->getY() * e->getX();
    }

    int getA() {return a;}
    int getB() {return b;}
    int getC() {return c;}

    void setA(int _a) {a = _a;}
    void setB(int _b) {b = _b;}
    void setC(int _c) {c = _c;}
};

class vertex_t {
  private:
    point_t* Vrh;
    edge_t* Brid;
    int lijevi;
  public:
    vertex_t() {lijevi = 0;}
    vertex_t(point_t *vrh, edge_t *brid, int l) : Vrh(vrh), Brid(brid), lijevi(l) {}

    ~vertex_t() {
      delete Vrh;
      delete Brid;
    }

    point_t* getVrh() {return Vrh;}
    edge_t* getBrid() {return Brid;}
    int isLeft() {return lijevi;}
};

// kraj bloka s pomocnim klasama

int provjeriKonveksnost(vector< vertex_t* > p) {
}

int provjeriPoziciju(vector< vertex_t* > p, point_t* t) {
}

void popuniPoligon(vector< vertex_t* > p) {
  int xmin, xmax, ymin, ymax;
  double L, D, x;

  int n = p.size();

  xmin = xmax = p[0]->getVrh()->getX();
  ymin = ymax = p[0]->getVrh()->getY();
  for (int i = 1; i < n; ++i) {
    xmin = min(xmin, p[i]->getVrh()->getX());
    xmax = max(xmax, p[i]->getVrh()->getX());
    ymin = min(ymin, p[i]->getVrh()->getY());
    ymax = max(ymax, p[i]->getVrh()->getY());
  }

  glBegin(GL_LINES);

  for (int y = ymin; y <= ymax; ++y) {
    L = xmin; D = xmax;

    for (int i = 0; i < n; ++i) {
      int j = (i + 1) % n;
      if (p[i]->getBrid()->getA() == 0) {
        if (p[i]->getVrh()->getY() == y) {
          if (p[i]->getVrh()->getX() > p[j]->getVrh()->getX()) {
            L = p[j]->getVrh()->getX();
            D = p[i]->getVrh()->getX();
          } else {
            L = p[i]->getVrh()->getX();
            D = p[j]->getVrh()->getX();
          }
          break;
        }
      } else {
        // provjera ako ima sjecista koja nas zanimaju
        int lo = p[i]->getVrh()->getY();
        int hi = p[j]->getVrh()->getY();

        if (lo > hi) {swap(lo, hi);}

        if (lo > y || hi < y) continue;

        x = -(y * p[i]->getBrid()->getB() + p[i]->getBrid()->getC()) / (double)p[i]->getBrid()->getA();

        if (p[i]->isLeft() == 1) {
          L = max(L, x);
        } else {
          D = min(D, x);
        }
      }
    }

    glVertex2i((int)L, y);
    glVertex2i((int)D, y);
  }
  glEnd();

  return;
}

class state_t {
  private:
    int width, height;
    int stanje;
    int konveksnost, popunjavanje;

    vector< vertex_t* > polygon;
    point_t* activePoint;

  public:
    state_t() {
      stanje = 1;
      konveksnost = popunjavanje = 0;
      activePoint = NULL;
    }

    ~state_t() {
      polygon.clear();
    }

    vector< vertex_t* > getPolygon() {
      return polygon;
    }

    int getPolygonSize() {
      return polygon.size();
    }

    void toggleStanje() {
      if (stanje == 2) {
        // obrisi sve podatke o poligonu
        polygon.clear();
        activePoint = NULL;

        // postavi zastice na false
        this->setPopunjavanje(0);
        this->setKonveksnost(0);
      } else if (stanje == 1 && polygon.size() > 1) {
        // spremi zadnju tocku
        point_t* p = polygon[0]->getVrh();
        polygon.push_back(new vertex_t(activePoint, new edge_t(activePoint, p), activePoint->getY() < p->getY()));
      }

      // toggle stanje
      stanje = 3 - stanje;
    }

    void toggleKonveksnost() {
      // omoguceno je samo u stanju 1
      if (this->stanje != 1) return;

      if (konveksnost == 0 && polygon.size() > 1) {
        // provjeri ako sve tocke do sad cine konveksni poligon
      }

      // toggle konveksnost
      konveksnost = 1 - konveksnost;
    }

    void togglePopunjavanje() {
      // omoguceno samo u stanju 1
      if (this->stanje != 1) return;

      // toggle popunjavanje
      popunjavanje = 1 - popunjavanje;
    }

    void setPopunjavanje(int _popunjavanje) {
      this->popunjavanje = _popunjavanje;
    }

    void setKonveksnost(int _konveksnost) {
      this->konveksnost = _konveksnost;
    }

    int getStanje() {return stanje;}
    int getPopunjavanje() {return popunjavanje;}
    int getKonveksnost() {return konveksnost;}

    point_t* getActivePoint() {return activePoint;}

    void addTocka(point_t *p) {
      if (activePoint != NULL) {
        // provjeri ako tocka p sacinjava konveksni mongokut
        polygon.push_back(new vertex_t(activePoint, new edge_t(activePoint, p), activePoint->getY() < p->getY()));
      }

      activePoint = p;
    }

    void setWidth(int w) {width = w;}
    void setHeight(int h) {height = h;}

    int getWidth() {return width;}
    int getHeight() {return height;}

} STATE;

int main(int argc, char **argv) {
  STATE.setWidth(500);
  STATE.setHeight(500);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(STATE.getWidth(), STATE.getHeight());

  glutInitWindowPosition(0, 0);
  glutCreateWindow("Zadatak 4.");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyboardPressed);
  glutMouseFunc(mouseClick);
  glutPassiveMotionFunc(mouseMoved);

  glutMainLoop();
  return 0;
}

void display() {
  if (STATE.getKonveksnost()) {
    // zelena boja
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  } else {
    // bijela boja
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  }
  glClear(GL_COLOR_BUFFER_BIT);

  // crtaj scenu
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
  glColor3f(0.0f, 0.0f, 0.0f);
  vector< vertex_t* > poly;

  switch (STATE.getStanje()) {
    case 1:
       // ispuni ako treba
      if (STATE.getPopunjavanje() == 1 && STATE.getPolygonSize() >= 1) {
        // activePoint -> kursor -> polygon[0]
        poly = STATE.getPolygon();

        poly.push_back(new vertex_t(STATE.getActivePoint(), new edge_t(STATE.getActivePoint(), kursor), STATE.getActivePoint()->getY() < kursor->getY()));
        poly.push_back(new vertex_t(kursor, new edge_t(kursor, poly[0]->getVrh()), kursor->getY() < poly[0]->getVrh()->getY()));

        popuniPoligon(poly);

      } else if (STATE.getPopunjavanje() == 0) {
        // crtanje poligona
        glBegin(GL_LINE_LOOP);
        // crtanje definiranih bridova
        for (
            vector< vertex_t* >::iterator
            it = STATE.getPolygon().begin();
            it != STATE.getPolygon().end();
            ++it
            ) {
          glVertex2i((*it)->getVrh()->getX(), (*it)->getVrh()->getY());
        }

        // dodaj zadnju tocku i poziciju kursora
        if (STATE.getActivePoint() != NULL) {
          glVertex2i(STATE.getActivePoint()->getX(), STATE.getActivePoint()->getY());
          glVertex2i(kursor->getX(), kursor->getY());
        }
        glEnd();
      }

     break;
    case 2:
      if (STATE.getPopunjavanje() == 1 && STATE.getPolygonSize() >= 3) {
        popuniPoligon(STATE.getPolygon());
      } else {
        // crtanje poligona
        glBegin(GL_LINE_LOOP);
        // crtanje definiranih bridova
        for (
            vector< vertex_t* >::iterator
            it = STATE.getPolygon().begin();
            it != STATE.getPolygon().end();
            ++it
            ) {
          glVertex2i((*it)->getVrh()->getX(), (*it)->getVrh()->getY());
        }

        glEnd();
      }
      break;
    default:
      warnx("Nedefinirano stanje!\n");
      break;
  }

}

void keyboardPressed(unsigned char key, int x, int y) {
  switch(key) {
    case 'k':
      STATE.toggleKonveksnost();
      break;
    case 'p':
      STATE.togglePopunjavanje();
      break;
    case 'n':
      STATE.toggleStanje();
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

void mouseMoved(int x, int y) {
  if (STATE.getStanje() == 1) {
    // oslobodi memorije
    if (kursor != NULL) delete kursor;
    kursor = new point_t(x, y);
    glutPostRedisplay();
  }
}

void mouseClick(int button, int status, int x, int y) {
  // ako je pritisnut lijevi gumb misa
  if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
    int status;
    point_t* tocka = new point_t(x, y);

    switch(STATE.getStanje()) {
      case 1:
        STATE.addTocka(tocka);
        break;
      case 2:
        // provjeri ako je tocka unutaar poligona i ispisi rezultat
        status = provjeriPoziciju(STATE.getPolygon(), tocka);
        if (status == 1) {
          printf("Tocka se nalazi unutar poligona!\n");
        } else if (status == -1) {
          printf("Tocka se nalazi izvan poligona\n");
        } else if (status == 0) {
          printf("Tocka se nalazi na rubu poligona\n");
        } else {
          warnx("Nedefiniran rezultat!\n");
        }

        // oslobodi memorije
        delete tocka;
        break;
      default:
        warnx("Nedefinirano stanje!\n");
        break;
    }

  }
  glutPostRedisplay();
}


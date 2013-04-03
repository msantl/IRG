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
} kursor;

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
  int iznad = 0, ispod = 0;
  int n = p.size();

  for (int i = 0; i < n; ++i) {
    int j = (i - 2 + n) % n;
    int r = p[j]->getBrid()->getA() * p[i]->getVrh()->getX() +
            p[j]->getBrid()->getB() * p[i]->getVrh()->getY() +
            p[j]->getBrid()->getC();

    if (r > 0) {
      iznad++;
    } else if (r < 0) {
      ispod++;
    }
  }

  return (iznad == 0) || (ispod == 0);
}

int provjeriPoziciju(vector< vertex_t* > p, point_t* t) {
  int iznad = 0, ispod = 0, na = 0;
  int n = p.size();

  for (int i = 0; i < n; ++i) {
    int r = p[i]->getBrid()->getA() * t->getX() +
            p[i]->getBrid()->getB() * t->getY() +
            p[i]->getBrid()->getC();

    if (r > 0) {
      iznad++;
    } else if (r < 0) {
      ispod++;
    } else {
      na++;
    }
  }

  if (na != 0) {
    return 0;
  } else if (iznad == 0) {
    return 1;
  } else if (ispod == 0) {
    return 1;
  } else {
    return -1;
  }
}

void popuniPoligon(vector< vertex_t* > p) {
  int n = p.size();
  int xmin, xmax, ymin, ymax;
  double L, D, x;

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
      int j = (i - 1 + n) % n;
      if (p[j]->getBrid()->getA() == 0) {
        // ako je brid vodoravan
        if (p[j]->getVrh()->getY() == y) {
          if (p[j]->getVrh()->getX() < p[i]->getVrh()->getX()) {
            L = p[j]->getVrh()->getX();
            D = p[i]->getVrh()->getX();
          } else {
            L = p[i]->getVrh()->getX();
            D = p[j]->getVrh()->getX();
          }
          break;
        }
      } else {
        // regularan brid
        x = (-p[j]->getBrid()->getB() * y - p[j]->getBrid()->getC()) /
          (double)p[j]->getBrid()->getA();

        if (p[j]->isLeft() == 1) {
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

    void toggleStanje() {
      if (stanje == 2) {
        // obrisi sve podatke o poligonu
        polygon.clear();
        activePoint = NULL;

        // postavi zastice na false
        this->setPopunjavanje(0);
        this->setKonveksnost(0);
      } else if (stanje == 1 && polygon.size() > 1) {
        // spremi i zadnju tocku
        point_t *p = polygon[0]->getVrh();
        edge_t *e = new edge_t(activePoint, p);
        int lijevi = activePoint->getY() < p->getY();

        polygon.push_back(new vertex_t(activePoint, e, lijevi));
      }

      // toggle stanje
      stanje = 3 - stanje;
    }

    void toggleKonveksnost() {
      // omoguceno je samo u stanju 1
      if (this->stanje != 1) return;

      if (konveksnost == 0 && polygon.size() > 1) {
        // provjeri ako sve tocke do sad cine konveksni poligon
        vector< vertex_t* > poly = polygon;
        point_t *p = polygon[0]->getVrh();
        edge_t *e = new edge_t(activePoint, p);
        int lijevi = activePoint->getY() < p->getY();

        poly.push_back(new vertex_t(activePoint, e, lijevi));

        // ako zelimo ukljuciti provjeru konveksnosti a trenutni poligon nije
        // konveksan nemoj dopustiti ukljucivanje
        if (provjeriKonveksnost(poly) == 0) {
          printf("Poligon nije konveksan, ne mogu ukljuciti opciju!\n");
          return;
        }
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
        // dodaj proslu tocku
        edge_t *e = new edge_t(activePoint, p);
        int lijevi = activePoint->getY() < p->getY();

        polygon.push_back(new vertex_t(activePoint, e, lijevi));

        // ako treba vrsiti provjeru
        if (konveksnost && polygon.size() > 1) {
          // provjeri novu tocku
          vector< vertex_t* > poly = polygon;
          e = new edge_t(p, polygon[0]->getVrh());
          lijevi = p->getY() < polygon[0]->getVrh()->getY();

          poly.push_back(new vertex_t(p, e, lijevi));

          if (provjeriKonveksnost(poly) == 0) {
            printf("Ne mogu dodati tocku jer poligon vise nece biti konveksan!\n");
            return;
          }
        }
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

  // temporary storage
  vector< vertex_t* > poly = STATE.getPolygon();
  int lijevi;
  edge_t* e;
  point_t *p, *kursor1, *kursor2;

  switch (STATE.getStanje()) {
    case 1:
       // ispuni ako treba
      if (STATE.getPopunjavanje() == 1 && poly.size() >= 1) {
        // activePoint -> kursor -> polygon[0]
        kursor1 = new point_t(kursor);
        kursor2 = new point_t(kursor);

        p = STATE.getActivePoint();
        e = new edge_t(p, kursor1);
        lijevi = p->getY() < kursor1->getY();

        poly.push_back(new vertex_t(p, e, lijevi));

        p = poly[0]->getVrh();
        e = new edge_t(kursor2, p);
        lijevi = kursor2->getY() < p->getY();

        poly.push_back(new vertex_t(kursor2, e, lijevi));

        popuniPoligon(poly);

      } else {
        // crtanje poligona
        glBegin(GL_LINE_LOOP);
        // crtanje definiranih bridova
        for (
            vector< vertex_t* >::iterator
            it = poly.begin();
            it != poly.end();
            ++it
            ) {
          glVertex2i((*it)->getVrh()->getX(), (*it)->getVrh()->getY());
        }

        // dodaj zadnju tocku i poziciju kursora
        if (STATE.getActivePoint() != NULL) {
          glVertex2i(STATE.getActivePoint()->getX(), STATE.getActivePoint()->getY());
          glVertex2i(kursor.getX(), kursor.getY());
        }
        glEnd();
      }

     break;
    case 2:
      if (STATE.getPopunjavanje() == 1 && poly.size() >= 3) {
        popuniPoligon(poly);
      } else {
        // crtanje poligona
        glBegin(GL_LINE_LOOP);
        // crtanje definiranih bridova
        for (
            vector< vertex_t* >::iterator
            it = poly.begin();
            it != poly.end();
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
    kursor = point_t(x, y);
    glutPostRedisplay();
  }
}

void mouseClick(int button, int status, int x, int y) {
  // ako je pritisnut lijevi gumb misa
  if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
    int status;
    vector< vertex_t* > poly;
    point_t* tocka = new point_t(x, y);

    switch(STATE.getStanje()) {
      case 1:
        STATE.addTocka(tocka);
        // tocka bude obrisana prilikom destrukcije poligona
        break;
      case 2:
        // provjeri ako je tocka unutaar poligona i ispisi rezultat
        poly = STATE.getPolygon();
        status = provjeriPoziciju(poly, tocka);
        if (status == 1) {
          printf("Tocka se nalazi unutar poligona!\n");
        } else if (status == -1) {
          printf("Tocka se nalazi izvan poligona\n");
        } else if (status == 0) {
          printf("Tocka se nalazi na rubu poligona\n");
        } else {
          warnx("Nedefiniran rezultat!\n");
        }

        break;
      default:
        warnx("Nedefinirano stanje!\n");
        break;
    }

  }
  glutPostRedisplay();
}


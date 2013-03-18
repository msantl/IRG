#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <GLUT/glut.h>
using namespace std;

// offset za kontrolnu liniju
#define OFFSET 5

// cohen sutherland konstante
#define YMAX 8
#define YMIN 4
#define XMAX 2
#define XMIN 1

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
    if (STATE.getOdsijecanje() == true) {
      // izracunaj vidljivi dio segmenta
    }

    // nacrtaj vidljivi dio segmenta
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

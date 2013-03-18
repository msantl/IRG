#include <cstdio>
#include <cstdlib>
#include <vector>
#include <GLUT/glut.h>
using namespace std;

class color_t {
  private:
    float R, G, B;
  public:

    color_t() {}
    color_t(float r, float g, float b) {R = r; G = g; B = b;}

    float getR() {return R;}
    float getG() {return G;}
    float getB() {return B;}
};

class colorContainer_t {
  private:
    int index;
    vector< color_t* > colors;
  public:
    colorContainer_t() {index = 0;}

    ~colorContainer_t() {
      for (vector< color_t* >::iterator it = colors.begin(); it != colors.end(); ++it ) {
        delete (*it);
      }

      colors.clear();
    }

    void addColor(color_t* color) {
      colors.push_back(color);
    }

    color_t* currColor() {
      return colors[index];
    }

    color_t* nextColor() {
      index = (index + 1) % colors.size();
      return colors[index];
    }

    color_t* prevColor() {
      index = (index - 1 + colors.size()) % colors.size();
      return colors[index];
    }
};

class trokut_t {
  private:
    int index;
    pair< int, int > p[3];
    color_t* color;
  public:
    trokut_t() {index = 0;}

    void setColor(color_t* _color) {color = _color;}

    void setPoint(pair<int, int> a, int n) {
      p[n] = a;
    }

    void addPoint(pair<int, int> a) {
      p[index] = a;
      index++;
    }

    color_t* getColor() {return color;}
    int getPointCount() {return index;}
    pair< int, int > getPoint(int n) {return p[n];}
};

void reshape(int width,int height);
void display();
void renderScene();
void mousePressed(int button, int state, int x, int y);
void mouseMoved(int x, int y);
void keyPressed(unsigned char key, int x, int y);

class state_t {
  private:
    trokut_t* aktivan;
    vector< trokut_t* > trokuti;
    colorContainer_t* colors;
  public:
    state_t() {
      aktivan = NULL;

      colors = new colorContainer_t();
      colors->addColor(new color_t(1.0f, 0.0f, 0.0f)); // red
      colors->addColor(new color_t(0.0f, 1.0f, 0.0f)); // green
      colors->addColor(new color_t(0.0f, 0.0f, 1.0f)); // blue
      colors->addColor(new color_t(0.0f, 1.0f, 1.0f)); // cyan
      colors->addColor(new color_t(1.0f, 1.0f, 0.0f)); // yellow
      colors->addColor(new color_t(1.0f, 0.0f, 1.0f)); // magneta
    }

    ~state_t() {
      delete aktivan;
      for (vector< trokut_t* >::iterator it = trokuti.begin(); it != trokuti.end(); ++it) {
        delete (*it);
      }
      trokuti.clear();

      delete colors;
    }

    trokut_t* getAktivan() {
      if (aktivan == NULL || aktivan->getPointCount() == 3) {
        aktivan = new trokut_t();
      }
      return aktivan;
    }

    vector< trokut_t* > getTrokut() {return trokuti;}

    color_t* getCurrColor() {return colors->currColor();}
    color_t* getNextColor() {return colors->nextColor();}
    color_t* getPrevColor() {return colors->prevColor();}

    void addTrokut(trokut_t* t) {
      trokuti.push_back(t);
    }

} STATE;

int main(int argc,char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Zadatak 2.2");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyPressed);
  glutMouseFunc(mousePressed);
  glutPassiveMotionFunc(mouseMoved);

  glutMainLoop();
}

void display(){
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  // crtanje scene
  renderScene();
  glutSwapBuffers();
}

void reshape(int width,int height){
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height); // otvor u prozoru
  glMatrixMode(GL_PROJECTION);                       // matrica projekcije
  glLoadIdentity();                                  // jedinicna matrica
  glOrtho(0, width-1, height-1, 0, 0, 1);            // okomita projekcija
  glMatrixMode(GL_MODELVIEW);                        // matrija projekcije
}

void renderScene(){
  // boja
  color_t* color;

  // velicina tocke
  glPointSize(1.0f);

  // nacrtaj postojece trokute
  vector< trokut_t* > trokuti = STATE.getTrokut();
  for (vector< trokut_t* >::iterator it = trokuti.begin(); it != trokuti.end(); ++it) {
    color = (*it)->getColor();
    glColor3f(color->getR(), color->getG(), color->getB());

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < 3; ++i) {
      pair< int, int > point = (*it)->getPoint(i);
      glVertex2i(point.first, point.second );
    }

    glEnd();
  }

  // nacrtaj trenutni trokut
  color = STATE.getCurrColor();
  glColor3f(color->getR(), color->getG(), color->getB());
  if (STATE.getAktivan()->getPointCount() == 2) {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; ++i) {
      pair< int, int > point = STATE.getAktivan()->getPoint(i);
      glVertex2i(point.first, point.second );
    }
    glEnd();
  } else if (STATE.getAktivan()->getPointCount() == 1){
    glBegin(GL_LINES);
    for (int i = 0; i < 2; ++i) {
      pair< int, int > point = STATE.getAktivan()->getPoint(i);
      glVertex2i(point.first, point.second );
    }
    glEnd();
  }

  // boja kvadrata
  color = STATE.getCurrColor();
  glColor3f(color->getR(), color->getG(), color->getB());

  glBegin(GL_QUADS);
  glVertex2i(0, 0);
  glVertex2i(0, 5);
  glVertex2i(5, 5);
  glVertex2i(5, 0);
  glEnd();
}

void keyPressed(unsigned char key, int x, int y) {
  switch(key) {
    case 'n':
      STATE.getNextColor();
      break;
    case 'p':
      STATE.getPrevColor();
      break;
    default:
      break;
  }

  trokut_t* aktivan = STATE.getAktivan();
  aktivan->setColor(STATE.getCurrColor());

  glutPostRedisplay();
}

void mousePressed(int button, int state, int x, int y) {
  pair< int, int > point(x, y);

  // pritisnuta je lijeva tipka misa
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    trokut_t* aktivan = STATE.getAktivan();

    aktivan->addPoint(point);
    aktivan->setColor(STATE.getCurrColor());

    if (aktivan->getPointCount() == 3) {
      STATE.addTrokut(aktivan);
    }
  }

  glutPostRedisplay();
}

void mouseMoved(int x, int y) {
  trokut_t* aktivan = STATE.getAktivan();
  int tocaka = aktivan->getPointCount();
  pair< int, int >point(x, y);

  switch(tocaka) {
    case 0:
      aktivan->setPoint(point, 0);
      aktivan->setPoint(point, 1);
      aktivan->setPoint(point, 2);
    case 1:
      // crtaj liniju od tocke do x, y
      aktivan->setPoint(point, 1);
      aktivan->setPoint(point, 2);
      break;
    case 2:
      // crtaj trokut sa trcim vrhom x, y
      aktivan->setPoint(point, 2);
      break;
    default:
      break;
  }

  glutPostRedisplay();
}

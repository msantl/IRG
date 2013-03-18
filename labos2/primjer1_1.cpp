#include <cstdio>
#include <cstdlib>
#include <GLUT/glut.h>

void reshape(int width,int height);
void display();
void renderScene();

int main(int argc,char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(200, 200);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Primjer1");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
}

void display(){
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  //crtanjescene:
  renderScene();
  glutSwapBuffers();
}

void reshape(int width,int height){
  glDisable(GL_DEPTH_TEST);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width-1, height-1, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

void renderScene(){
  glPointSize(1.0f);
  glColor3f(0.0f, 1.0f, 1.0f);
  glBegin(GL_POINTS);
  glVertex2i(0, 0);
  glVertex2i(2, 2);
  glVertex2i(4, 4);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex2i(50, 50);
  glVertex2i(150, 150);
  glVertex2i(50, 150);
  glVertex2i(50, 50);
  glEnd();
}

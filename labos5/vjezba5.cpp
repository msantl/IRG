#include "vjezba5.h"

#include <cstdio>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

vector< string > split(string str, string delimiter) {
  str += delimiter;
  vector< string > ret;
  int n = str.length();
  int m = delimiter.length();
  int last = 0;

  for (int i = 0; i + m <= n;) {
    if (str.substr(i, m) == delimiter) {
      ret.push_back(str.substr(last, i - last));
      last = i + m;
      i = i + m;
    } else {
      i = i + 1;
    }
  }

  return ret;
}

float string2float(string a) {
  stringstream ss;
  float x;
  ss << a;
  ss >> x;
  return x;
}

Vertex3D::Vertex3D() : x(0), y(0) {}
Vertex3D::Vertex3D(float _x, float _y) : x(_x), y(_y) {}
Vertex3D::Vertex3D(Vertex3D* a) {
  this->x = a->x;
  this->y = a->y;
}
void Vertex3D::setX(float _x) {this->x = _x;}
void Vertex3D::setY(float _y) {this->y = _y;}
float Vertex3D::getX() {return this->x;}
float Vertex3D::getY() {return this->y;}

Face3D::Face3D() {
  this->vrhovi[0] = NULL;
  this->vrhovi[1] = NULL;
  this->vrhovi[2] = NULL;
}
Face3D::Face3D(Vertex3D* v1, Vertex3D* v2, Vertex3D *v3) {
  this->vrhovi[0] = new Vertex3D(v1);
  this->vrhovi[1] = new Vertex3D(v2);
  this->vrhovi[2] = new Vertex3D(v3);
}
Face3D::Face3D(Face3D* a) {
  this->vrhovi[0] = new Vertex3D(a->getVrh(0));
  this->vrhovi[1] = new Vertex3D(a->getVrh(1));
  this->vrhovi[2] = new Vertex3D(a->getVrh(2));
}
Face3D::~Face3D() {
  delete this->vrhovi[0];
  delete this->vrhovi[1];
  delete this->vrhovi[2];
}
Vertex3D* Face3D::getVrh(int x) {
  assert(x >= 0 || x <= 2);
  return this->vrhovi[x];
}

ObjectModel::ObjectModel() {}
ObjectModel::~ObjectModel() {
  // obrisi tocke
  for (
    vector< Vertex3D* >::iterator
    it = this->vrhovi.begin();
    it != this->vrhovi.end();
    ++it
  ) {
    delete (*it);
  }

  // obrisi trokute
  for (
    vector< Face3D* >::iterator
    it = this->trokuti.begin();
    it != this->trokuti.end();
    ++it
  ) {
    delete (*it);
  }

}
ObjectModel* ObjectModel::copy() {
  ObjectModel* novi = new ObjectModel();
  // kopiraj tocke
  for (
    vector< Vertex3D* >::iterator
    it = this->vrhovi.begin();
    it != this->vrhovi.end();
    ++it
  ) {
    Vertex3D* tocka = new Vertex3D(*it);
    novi->addVrh(tocka);
  }

  // kopiraj trokute
  for (
    vector< Face3D* >::iterator
    it = this->trokuti.begin();
    it != this->trokuti.end();
    ++it
  ) {
    Face3D* trokut = new Face3D(*it);
    novi->addTrokut(trokut);
  }

  return novi;
}
void ObjectModel::addVrh(Vertex3D* a) {
  this->vrhovi.push_back(a);
}
void ObjectModel::addTrokut(Face3D* a) {
  this->trokuti.push_back(a);
}
string ObjectModel::dumpToOBJ() {
}
void ObjectModel::normalize() {
}

void parseFile(char *filename, ObjectModel *obj) {
  FILE *f = fopen(filename, "r");

  fclose(f);
}

int main(int argc, char** argv) {
  ObjectModel *model = new ObjectModel();

  parseFile(argv[1], model);

  delete model;
  return 0;
}

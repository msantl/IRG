#include "vjezba5.h"

#include <cstdio>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
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

int string2int(string a) {
  stringstream ss;
  int x;
  ss << a;
  ss >> x;
  return x;
}

float string2float(string a) {
  stringstream ss;
  float x;
  ss << a;
  ss >> x;
  return x;
}

Vertex3D::Vertex3D() : x(0), y(0), z(0) {}
Vertex3D::Vertex3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
Vertex3D::Vertex3D(Vertex3D* a) {
  this->x = a->x;
  this->y = a->y;
  this->z = a->z;
}
void Vertex3D::setX(float _x) {this->x = _x;}
void Vertex3D::setY(float _y) {this->y = _y;}
void Vertex3D::setZ(float _z) {this->z = _z;}
float Vertex3D::getX() {return this->x;}
float Vertex3D::getY() {return this->y;}
float Vertex3D::getZ() {return this->z;}

Face3D::Face3D() {
  this->indexes[0] = -1;
  this->indexes[1] = -1;
  this->indexes[2] = -1;
}
Face3D::Face3D(int x, int y, int z) {
  this->indexes[0] = x;
  this->indexes[1] = y;
  this->indexes[2] = z;
}
Face3D::Face3D(Face3D* a) {
  this->indexes[0] = a->indexes[0];
  this->indexes[1] = a->indexes[1];
  this->indexes[2] = a->indexes[2];
}
int Face3D::getVrhID(int x) {
  assert(x >= 0 || x <= 2);
  return this->indexes[x];
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
Vertex3D* ObjectModel::getVrh(int x) {
  assert(x >= 0 && x < (int)vrhovi.size());
  return this->vrhovi[x];
}
Face3D* ObjectModel::getTrokut(int x) {
  assert(x >= 0 && x < (int)trokuti.size());
  return this->trokuti[x];
}
vector< Vertex3D* > ObjectModel::getVrhovi() {
  return this->vrhovi;
}
vector< Face3D* > ObjectModel::getTrokuti() {
  return this->trokuti;
}

string ObjectModel::dumpToOBJ() {
  string dump = "# OBJ file dump\n";
  char buff[1024];
  // add comment
  // add vrhovi
  for (
    vector< Vertex3D* >::iterator
    it = this->vrhovi.begin();
    it != this->vrhovi.end();
    ++it
  ) {
    sprintf(
      buff,
      "v %f %f %f\n",
      (*it)->getX(),
      (*it)->getY(),
      (*it)->getZ()
    );

    dump = dump + string(buff);
  }

  // add trokuti
  for (
    vector< Face3D* >::iterator
    it = this->trokuti.begin();
    it != this->trokuti.end();
    ++it
  ) {
    sprintf(
      buff,
      "f %d %d %d\n",
      (*it)->getVrhID(0) + 1,
      (*it)->getVrhID(1) + 1,
      (*it)->getVrhID(2) + 1
    );

    dump = dump + string(buff);
  }

  return dump;
}
void ObjectModel::normalize() {
  bool first = true;
  float xmin, xmax;
  float ymin, ymax;
  float zmin, zmax;
  float x_sr, y_sr, z_sr;
  float M;

  // nadji minimum
  for (
    vector< Vertex3D* >::iterator
    it = this->vrhovi.begin();
    it != this->vrhovi.end();
    ++it
  ) {
    if (first) {
      xmin = xmax = (*it)->getX();
      ymin = ymax = (*it)->getY();
      zmin = zmax = (*it)->getZ();
    } else {
      xmin = min(xmin, (*it)->getX());
      xmax = max(xmax, (*it)->getX());
      ymin = min(ymin, (*it)->getY());
      ymax = max(ymax, (*it)->getY());
      zmin = min(zmin, (*it)->getZ());
      zmax = max(zmax, (*it)->getZ());
    }
    first = false;
  }

  // izracunaj srednju vrijednost
  x_sr = (xmin + xmax) / 2.;
  y_sr = (ymin + ymax) / 2.;
  z_sr = (zmin + zmax) / 2.;

  // maksimalni raspon
  M = max(max(xmax - xmin, ymax - ymin), zmax - zmin);

  // translatiraj
  for (
    vector< Vertex3D* >::iterator
    it = this->vrhovi.begin();
    it != this->vrhovi.end();
    ++it
  ) {
    (*it)->setX((*it)->getX() - x_sr);
    (*it)->setY((*it)->getY() - y_sr);
    (*it)->setZ((*it)->getZ() - z_sr);
  }
  // skaliraj
  for (
    vector< Vertex3D* >::iterator
    it = this->vrhovi.begin();
    it != this->vrhovi.end();
    ++it
  ) {
    (*it)->setX(2 * (*it)->getX() / M);
    (*it)->setY(2 * (*it)->getY() / M);
    (*it)->setZ(2 * (*it)->getZ() / M);
  }
  return;
}

void parseFile(char *filename, ObjectModel *obj) {
  ifstream file;
  string buff;
  vector< string > s_buff;

  file.open(filename, fstream::in);

  while (getline(file, buff)) {
    s_buff = split(buff, " ");
    if (s_buff[0] == "#") {
      // komentar -> ignorirati
      continue;
    } else if (s_buff[0] == "g") {
      // grupa -> ime grupe
    } else if (s_buff[0] == "v") {
      // vrh
      float x = string2float(s_buff[1]);
      float y = string2float(s_buff[2]);
      float z = string2float(s_buff[3]);
      Vertex3D* vrh = new Vertex3D(x, y, z);

      // dodaj ga u model
      obj->addVrh(vrh);

    } else if (s_buff[0] == "f") {
      // face
      // indeksi pocinju od 1
      int x = string2int(s_buff[1]) - 1;
      int y = string2int(s_buff[2]) - 1;
      int z = string2int(s_buff[3]) - 1;
      Face3D* trokut = new Face3D(x, y, z);

      // dodaj ga u model
      obj->addTrokut(trokut);

    }
  }

  file.close();
}

int odnosObjectVertex(ObjectModel* obj, Vertex3D* ver) {
  // 1 - unutar
  // 0 - na rubu
  // -1 izvan

  int iznad = 0, ispod = 0, na = 0;
  vector< Face3D* > trokuti = obj->getTrokuti();

  for (
    vector< Face3D* >::iterator
    it = trokuti.begin();
    it != trokuti.end();
    ++it
  ) {
    // indeksi vrhova
    int i = (*it)->getVrhID(0);
    int j = (*it)->getVrhID(1);
    int k = (*it)->getVrhID(2);

    // medju rezultati
    float v2x = obj->getVrh(j)->getX() - obj->getVrh(i)->getX();
    float v2y = obj->getVrh(j)->getY() - obj->getVrh(i)->getY();
    float v2z = obj->getVrh(j)->getZ() - obj->getVrh(i)->getZ();
    float v3x = obj->getVrh(k)->getX() - obj->getVrh(i)->getX();
    float v3y = obj->getVrh(k)->getY() - obj->getVrh(i)->getY();
    float v3z = obj->getVrh(k)->getZ() - obj->getVrh(i)->getZ();
    // n = (V2 - V1) x (V3 - V1)
    // izracun komponenata normale
    float a = v2y * v3z - v3y * v2z;
    float b = - (v2x * v3z - v3x * v2z);
    float c = v2x * v3y - v3x * v2y;

    float d = -a * obj->getVrh(i)->getX()
              -b * obj->getVrh(i)->getY()
              -c * obj->getVrh(i)->getZ();

    // odnos tocke i ravnine
    float r = a * ver->getX() +
              b * ver->getY() +
              c * ver->getZ() +
              d;

    if (r > 0) {
      iznad += 1;
    } else if (r < 0) {
      ispod += 1;
    } else {
      na += 1;
    }
  }

  if ((iznad == 0 || ispod == 0) && na != 0) {
    return 0;
  } else if (ispod == 0) {
    return 1;
  } else if (iznad == 0) {
    return 1;
  } else {
    return -1;
  }
}

int main(int argc, char** argv) {
  assert(argc == 2);
  ObjectModel *model = new ObjectModel();
  vector< string > s_buff;
  string buff;

  parseFile(argv[1], model);

  while (1) {
    getline(cin, buff);
    s_buff = split(buff, " ");

    if (s_buff[0] == "quit") {
      break;
    } else if (s_buff[0] == "normiraj") {
      model->normalize();
      cout << model->dumpToOBJ() << endl;
    } else {
      float x = string2float(s_buff[0]);
      float y = string2float(s_buff[1]);
      float z = string2float(s_buff[2]);

      Vertex3D* tocka = new Vertex3D(x, y, z);

      int status = odnosObjectVertex(model, tocka);

      printf(
        "Tocka (%.2lf %.2lf %.2lf) je %s tijela.\n",
        x,
        y,
        z,
        status ? (status == 1 ? "unutar" : "izvan") : "na rubu"
      );

      delete tocka;
    }
  }

  delete model;
  return 0;
}

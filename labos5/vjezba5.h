#ifndef VJEZBA5_H
#define VJEZBA5_H

#include <vector>
#include <string>

std::vector< std::string > split(std::string, std::string);
float string2float(std::string);

class Vertex3D {
  private:
    float x, y;
  public:
    Vertex3D();
    Vertex3D(float, float);
    // copy constructor
    Vertex3D(Vertex3D*);

    void setX(float);
    void setY(float);

    float getX();
    float getY();
};

class Face3D {
  private:
    Vertex3D* vrhovi[3];
  public:
    Face3D();
    Face3D(Vertex3D*, Vertex3D*, Vertex3D*);
    // copy contructor
    Face3D(Face3D*);

    ~Face3D();

    Vertex3D* getVrh(int);
};

class ObjectModel {
  private:
    std::vector< Vertex3D* > vrhovi;
    std::vector< Face3D* > trokuti;
  public:
    ObjectModel();
    ~ObjectModel();

    void addVrh(Vertex3D*);
    void addTrokut(Face3D*);

    std::vector< Vertex3D* > getVrhovi();
    std::vector< Face3D* > getTrokuti();

    // dodatno
    ObjectModel* copy();
    std::string dumpToOBJ();
    void normalize();

};

#endif

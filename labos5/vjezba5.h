#ifndef VJEZBA5_H
#define VJEZBA5_H

#include <vector>
#include <string>

std::vector< std::string > split(std::string, std::string);
float string2float(std::string);
int string2int(std::string);

class Vertex3D {
  private:
    float x, y, z;
  public:
    Vertex3D();
    Vertex3D(float, float, float);
    // copy constructor
    Vertex3D(Vertex3D*);

    void setX(float);
    void setY(float);
    void setZ(float);

    float getX();
    float getY();
    float getZ();
};

class Face3D {
  private:
    int indexes[3];
  public:
    Face3D();
    Face3D(int, int, int);
    // copy constructor
    Face3D(Face3D*);

    int getVrhID(int);
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

    Vertex3D* getVrh(int);
    Face3D* getTrokut(int);

    std::vector< Vertex3D* > getVrhovi();
    std::vector< Face3D* > getTrokuti();

    // dodatno
    ObjectModel* copy();
    std::string dumpToOBJ();
    void normalize();

};

#endif

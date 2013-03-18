#ifndef VECTOR_H
#define VECTOR_H

#include "AbstractVector.h"
#include <vector>
#include <string>

class Vector : public AbstractVector {
  private:
    std::vector< double > *elements;
    int dimension;
    bool readOnly, freeOnDelete;
  public:
    Vector();
    ~Vector();

    Vector(std::vector< double > *el);
    Vector(bool, bool, std::vector< double > *el);

    double get(int);
    IVector* set(int, double);
    int getDimension();
    IVector* copy();
    IVector* newInstance(int);

    static Vector* parseSimple(std::string);
};

 #endif

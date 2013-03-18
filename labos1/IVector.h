#ifndef IVECTOR_H
#define IVECTOR_H

#include "IRG.h"

#include "IMatrix.h"
#include <string>

class IVector {
  public:
    virtual double get(int) = 0;
    virtual IVector* set(int, double) = 0;
    virtual int getDimension() = 0;
    virtual IVector* copy() = 0;
    virtual IVector* copyPart(int) = 0;
    virtual IVector* newInstance(int) = 0;
    virtual IVector* add(IVector*) = 0;
    virtual IVector* nAdd(IVector*) = 0;
    virtual IVector* sub(IVector*) = 0;
    virtual IVector* nSub(IVector*) = 0;
    virtual IVector* scalarMultiply(double) = 0;
    virtual IVector* nScalarMultiply(double) = 0;
    virtual double norm() = 0;
    virtual IVector* normalize() = 0;
    virtual IVector* nNormalize() = 0;
    virtual double cosine(IVector*) = 0;
    virtual double scalarProduct(IVector*) = 0;
    virtual IVector* nVectorProduct(IVector*) = 0;
    virtual IVector* nFromHomogenus() = 0;
    virtual IMatrix* toRowMatrix(bool) = 0;
    virtual IMatrix* toColumnMatrix(bool) = 0;
    virtual double* toArray() = 0;

    virtual std::string toString() = 0;
};

#endif

#ifndef ABSTRACTVECTOR_H
#define ABSTRACTVECTOR_H

#include "IRG.h"

#include "IVector.h"
#include "IMatrix.h"
#include <string>

class AbstractVector : virtual public IVector {
  public:
    AbstractVector();

    IVector* copyPart(int);

    IVector* add(IVector*);
    IVector* nAdd(IVector*);
    IVector* sub(IVector*);
    IVector* nSub(IVector*);
    IVector* scalarMultiply(double);
    IVector* nScalarMultiply(double);
    double norm();
    IVector* normalize();
    IVector* nNormalize();
    double cosine(IVector*);
    double scalarProduct(IVector*);
    IVector* nVectorProduct(IVector*);
    IVector* nFromHomogenus();

    IMatrix* toRowMatrix(bool);
    IMatrix* toColumnMatrix(bool);

    double* toArray();

    std::string toString();
};

#endif

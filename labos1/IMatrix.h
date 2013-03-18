#ifndef IMATRIX_H
#define IMATRIX_H

#include "IRG.h"

#include "IVector.h"
#include <string>

class IMatrix {
  public:
    virtual int getRowsCount() = 0;
    virtual int getColsCount() = 0;
    virtual double get(int, int) = 0;
    virtual IMatrix* set(int, int, double) = 0;
    virtual IMatrix* copy() = 0;
    virtual IMatrix* newInstance(int, int) = 0;
    virtual IMatrix* nTranspose(bool) = 0;
    virtual IMatrix* add(IMatrix*) = 0;
    virtual IMatrix* nAdd(IMatrix*) = 0;
    virtual IMatrix* sub(IMatrix*) = 0;
    virtual IMatrix* nSub(IMatrix*) = 0;
    virtual IMatrix* nMultiply(IMatrix*) = 0;
    virtual double determinant() = 0;
    virtual IMatrix* subMatrix(int, int, bool) = 0;
    virtual IMatrix* nInvert() = 0;
    virtual double* toArray() = 0;
    virtual IVector* toVector(bool) = 0;

    virtual std::string toString() = 0;
};

#endif

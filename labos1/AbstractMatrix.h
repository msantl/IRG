#ifndef ABSTRACTMATRIX_H
#define ABSTRACTMATRIX_H

#include "IRG.h"

#include "IMatrix.h"
#include "IVector.h"

#include <string>

class AbstractMatrix : virtual public IMatrix {
  public:
    AbstractMatrix();
    IMatrix* nTranspose(bool);
    IMatrix* add(IMatrix*);
    IMatrix* nAdd(IMatrix*);
    IMatrix* sub(IMatrix*);
    IMatrix* nSub(IMatrix*);
    IMatrix* nMultiply(IMatrix*);
    double determinant();
    IMatrix* subMatrix(int, int, bool);
    IMatrix* nInvert();
    double* toArray();

    std::string toString();

    IVector* toVector(bool);

};

#endif

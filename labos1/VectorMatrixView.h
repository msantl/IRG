#ifndef VECTORMATRIXVIEW_H
#define VECTORMATRIXVIEW_H

#include "IRG.h"

#include "IVector.h"
#include "IMatrix.h"

#include "AbstractVector.h"

class VectorMatrixView : public AbstractVector {
  private:
    int dimension;
    bool rowMatrix;
    IMatrix *view;
  public:
    VectorMatrixView();
    VectorMatrixView(IMatrix*);
    double get(int);
    IVector* set(int, double);
    int getDimension();
    IVector* copy();
    IVector* newInstance(int);
};

#endif

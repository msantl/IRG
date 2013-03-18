#ifndef MATRIXTRANSPOSEVIEW_H
#define MATRIXTRANSPOSEVIEW_H

#include "IRG.h"

#include "AbstractMatrix.h"

class MatrixTransposeView : public AbstractMatrix {
  private:
    IMatrix *view;
  public:
    MatrixTransposeView();
    MatrixTransposeView(IMatrix*);
    int getRowsCount();
    int getColsCount();
    double get(int, int);
    IMatrix* set(int, int, double);
    IMatrix* copy();
    IMatrix* newInstance(int, int);
    double* toArray();
};

#endif

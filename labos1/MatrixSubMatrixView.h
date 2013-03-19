#ifndef MATRIXSUBMATRIXVIEW_H
#define MATRIXSUBMATRIXVIEW_H

#include "IRG.h"

#include "AbstractMatrix.h"

#include <vector>

class MatrixSubMatrixView : public AbstractMatrix {
  private:
    std::vector< int > rowIndexes;
    std::vector< int > colIndexes;

    IMatrix* view;

    MatrixSubMatrixView(IMatrix*, std::vector< int >, std::vector< int >);

  public:
    MatrixSubMatrixView();
    MatrixSubMatrixView(IMatrix*, int, int);

    int getRowsCount();
    int getColsCount();
    double get(int, int);
    IMatrix* set(int, int, double);
    IMatrix* copy();
    IMatrix* newInstance(int, int);
    IMatrix* subMatrix(int, int, bool);
};

#endif

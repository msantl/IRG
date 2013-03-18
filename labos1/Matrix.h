#ifndef MATRIX_H
#define MATRIX_H

#include "IRG.h"

#include "AbstractMatrix.h"
#include <vector>
#include <string>

class Matrix : public AbstractMatrix {
  private:
    int rows, cols;
    std::vector< std::vector< double > > *elements;
  public:
    Matrix();
    ~Matrix();

    Matrix(int, int);
    Matrix(int, int, std::vector< std::vector< double > >*, bool);

    int getRowsCount();
    int getColsCount();
    double get(int, int);
    IMatrix* set(int, int, double);
    IMatrix* copy();
    IMatrix* newInstance(int, int);

    static Matrix* parseSimple(std::string);
};

#endif

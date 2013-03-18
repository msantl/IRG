#include "AbstractMatrix.h"
#include "MatrixTransposeView.h"
#include "MatrixSubMatrixView.h"
#include <string>
#include <cassert>
#include <cmath>

using namespace std;

AbstractMatrix::AbstractMatrix() {}

IMatrix* AbstractMatrix::nTranspose(bool liveView) {
  // TODO
  return NULL;
}

IMatrix* AbstractMatrix::add(IMatrix* m2) {
  assert(this->getRowsCount() == m2->getRowsCount());
  assert(this->getColsCount() == m2->getColsCount());

  for (int i = 0; i < this->getRowsCount(); ++i) {
    for (int j = 0; j < this->getColsCount(); ++j) {
      this->set(i, j, this->get(i, j) + m2->get(i, j));
    }
  }
  return this;
}

IMatrix* AbstractMatrix::nAdd(IMatrix* m2) {
  IMatrix* m1 = this->copy();

  return m1->add(m2);
}

IMatrix* AbstractMatrix::sub(IMatrix* m2) {
  assert(this->getRowsCount() == m2->getRowsCount());
  assert(this->getColsCount() == m2->getColsCount());

  for (int i = 0; i < this->getRowsCount(); ++i) {
    for (int j = 0; j < this->getColsCount(); ++j) {
      this->set(i, j, this->get(i, j) - m2->get(i, j));
    }
  }
  return this;
}

IMatrix* AbstractMatrix::nSub(IMatrix* m2) {
  IMatrix* m1 = this->copy();

  return m1->sub(m2);
}

IMatrix* AbstractMatrix::nMultiply(IMatrix* m2) {
  assert(this->getColsCount() == m2->getRowsCount());

  IMatrix* m1 = this->newInstance(this->getRowsCount(), m2->getColsCount());
  double suma;

  for (int i = 0; i < this->getRowsCount(); ++i) {
    for (int j = 0; j < m2->getColsCount(); ++j) {
      suma = 0;
      for (int k = 0; k < this->getColsCount(); ++k) {
        suma += this->get(i , k) * m2->get(k, j);
      }
      m1->set(i, j, suma);
    }
  }

  return m1;
}

double AbstractMatrix::determinant() {
  assert(this->getRowsCount() == this->getColsCount());
  assert(this->getRowsCount() < 4); // sve vise je previse

  double res = 0;

  switch(this->getRowsCount()) {
    case 1:
      res = this->get(0, 0);
      break;
    case 2:
      res = this->get(0, 0) * this->get(1, 1) - this->get(0, 1) * this->get(1, 0);
      break;
    case 3:
      res = this->get(0, 0) * this->get(1, 1) * this->get(2, 2)
          + this->get(0, 1) * this->get(1, 2) * this->get(2, 0)
          + this->get(0, 2) * this->get(1, 0) * this->get(2, 1)
          - this->get(0, 2) * this->get(1, 1) * this->get(2, 0)
          - this->get(0, 1) * this->get(1, 0) * this->get(2, 2)
          - this->get(0, 0) * this->get(1, 2) * this->get(2, 1);
      break;
    default:
      res = 0;
  }

  return res;
}

IMatrix* AbstractMatrix::subMatrix(int i, int j, bool liveView) {
  // TODO
  return NULL;
}

IMatrix* AbstractMatrix::nInvert() {
  double det = this->determinant();
  assert(fabs(det) > EPS);
  return NULL;
}

double* AbstractMatrix::toArray() {
  double *ret = new double[this->getRowsCount() * this->getColsCount()];

  for (int i = 0; i < this->getRowsCount(); ++i) {
    for (int j = 0; j < this->getColsCount(); ++j) {
      ret[i * this->getColsCount() + j] = this->get(i, j);
    }
  }

  return ret;
}

string AbstractMatrix::toString() {
  string str, row;
  char buff[20];

  for (int i = 0; i < this->getRowsCount(); ++i) {
    row = "";

    for (int j = 0; j < this->getColsCount(); ++j) {
      sprintf(buff, "%3lf, ", this->get(i, j));

      row = row + string(buff);
    }

    str = str + "[" + row + "]" + '\n';
  }

  return str;
}

IVector* AbstractMatrix::toVector(bool liveView) {
  // TODO
  return NULL;
}

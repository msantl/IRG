#include "MatrixVectorView.h"

#include <cassert>

using namespace std;

MatrixVectorView::MatrixVectorView() {}

MatrixVectorView::MatrixVectorView(IVector *v, bool viewType) {
 asRowMatrix = viewType;
 view = v;
}

int MatrixVectorView::getRowsCount() {
  if (asRowMatrix) {
    return 1;
  } else {
    return view->getDimension();
  }
}

int MatrixVectorView::getColsCount() {
  if (asRowMatrix) {
    return view->getDimension();
  } else {
    return 1;
  }
}

double MatrixVectorView::get(int i, int j) {
  if (asRowMatrix) {
    assert(i == 1);
    return view->get(j);
  } else {
    assert(j == 1);
    return view->get(i);
  }
}

IMatrix* MatrixVectorView::set(int i, int j, double v) {
  if (asRowMatrix) {
    assert(i == 1);
    view->set(j, v);
  } else {
    assert(j == 1);
    view->set(i, v);
  }

  return this;
}

IMatrix* MatrixVectorView::copy() {
  IMatrix* m1 = new MatrixVectorView(view->copy(), asRowMatrix);
  return m1;
}

IMatrix* MatrixVectorView::newInstance(int r, int c) {
  int d = r == 1 ? c : r;
  return new MatrixVectorView(view->newInstance(d), r == 1 ? 1 : 0);
}

#include "VectorMatrixView.h"

using namespace std;

VectorMatrixView::VectorMatrixView() {}

VectorMatrixView::VectorMatrixView(IMatrix *v) {
  view = v;
  dimension = v->getRowsCount() * v->getColsCount();

  if (v->getRowsCount() == 1) {
    rowMatrix = true;
  } else {
    rowMatrix = false;
  }
}

double VectorMatrixView::get(int n) {
  if (rowMatrix) {
    return view->get(0, n);
  } else {
    return view->get(n, 0);
  }
}

IVector* VectorMatrixView::set(int n, double v) {
  if (rowMatrix) {
    view->set(0, n, v);
  } else {
    view->set(n, 0, v);
  }

  return this;
}

int VectorMatrixView::getDimension() {
  return this->dimension;
}

IVector* VectorMatrixView::copy() {
  IVector* v1 = new VectorMatrixView(view->copy());
  return v1;
}

IVector* VectorMatrixView::newInstance(int d) {
  if (rowMatrix) {
    return new VectorMatrixView(view->newInstance(1, d));
  } else {
    return new VectorMatrixView(view->newInstance(d, 1));
  }
}

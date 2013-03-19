#include "MatrixTransposeView.h"

using namespace std;

MatrixTransposeView::MatrixTransposeView() {}

MatrixTransposeView::MatrixTransposeView(IMatrix* m2) {
  view = m2;
}

int MatrixTransposeView::getRowsCount() {
  return view->getColsCount();
}

int MatrixTransposeView::getColsCount() {
  return view->getRowsCount();
}

double MatrixTransposeView::get(int i, int j) {
  return view->get(j ,i);
}

IMatrix* MatrixTransposeView::set(int i, int j, double v) {
  return view->set(j, i, v);
}

IMatrix* MatrixTransposeView::copy() {
  return new MatrixTransposeView(*this);
}

IMatrix* MatrixTransposeView::newInstance(int i, int j) {
  return new MatrixTransposeView(view->newInstance(view->getRowsCount(), view->getColsCount()));
}

double* MatrixTransposeView::toArray() {
  double* res = new double[view->getRowsCount() * view->getColsCount()];

  for (int i = 0; i < view->getRowsCount(); ++i) {
    for (int j = 0; j < view->getColsCount(); ++j) {
      res[i * this->getColsCount() + j] = this->get(i, j);
    }
  }

  return res;
}

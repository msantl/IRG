#include "MatrixSubMatrixView.h"

using namespace std;

MatrixSubMatrixView::MatrixSubMatrixView() {}

MatrixSubMatrixView::MatrixSubMatrixView(IMatrix* m2, int x, int y) {
  view = m2;

  for (int i = 0; i < m2->getRowsCount(); ++i) {
    if (i != x) {
      rowIndexes.push_back(i);
    }
  }
  for (int j = 0; j < m2->getColsCount(); ++j) {
    if (j != y) {
      colIndexes.push_back(j);
    }
  }
}

MatrixSubMatrixView::MatrixSubMatrixView(IMatrix* m2, vector< int >rows, vector< int > cols) {
  rowIndexes = rows;
  colIndexes = cols;

  view = m2;
}

int MatrixSubMatrixView::getRowsCount() {
  return (int)rowIndexes.size();
}

int MatrixSubMatrixView::getColsCount() {
  return (int)colIndexes.size();
}

double MatrixSubMatrixView::get(int i, int j) {
  return view->get(rowIndexes.at(i), colIndexes.at(j));
}

IMatrix* MatrixSubMatrixView::set(int i, int j, double v) {
  return view->set(rowIndexes.at(i), colIndexes.at(j), v);
}

IMatrix* MatrixSubMatrixView::copy() {
  IMatrix* m1 = new MatrixSubMatrixView(view->copy(), rowIndexes, colIndexes);
  return m1;
}

IMatrix* MatrixSubMatrixView::newInstance(int i, int j) {
  return new MatrixSubMatrixView(view->newInstance(view->getRowsCount(), view->getColsCount()), i, j);
}

IMatrix* MatrixSubMatrixView::subMatrix(int x, int y, bool liveView) {
  vector< int > rows;
  vector< int > cols;

  for (int i = 0; i < (int)rowIndexes.size(); ++i) {
    if (i != x) {
      rows.push_back(i);
    }
  }

  for (int j = 0; j < (int)colIndexes.size(); ++j) {
    if (j != y) {
      rows.push_back(j);
    }
  }

  IMatrix* m1;
  if (liveView) {
    m1 = new MatrixSubMatrixView(view, rows, cols);
  } else {
    m1 = new MatrixSubMatrixView(view->copy(), rows, cols);

  }
  return m1;
}

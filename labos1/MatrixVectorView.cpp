#include "MatrixVectorView.h"

using namespace std;

MatrixVectorView::MatrixVectorView() {
}

MatrixVectorView::MatrixVectorView(IVector *v, bool viewType) {
}

int MatrixVectorView::getRowsCount() {
}

int MatrixVectorView::getColsCount() {
}

double MatrixVectorView::get(int i, int j) {
}

IMatrix* MatrixVectorView::set(int i, int j, double v) {
}

IMatrix* MatrixVectorView::copy() {
}

IMatrix* MatrixVectorView::newInstance(int r, int c) {
}

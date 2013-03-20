#include "Matrix.h"
#include <algorithm>
#include <cassert>

using namespace std;

Matrix::Matrix() {
  rows = cols = 0;
  elements = NULL;
}

Matrix::~Matrix() {
  delete elements;
}

Matrix::Matrix(int r, int c) {
  rows = r;
  cols = c;
  elements = new vector< vector< double > >(r, vector< double >(c));
}

Matrix::Matrix(int r, int c, vector< vector< double > > *el, bool freeOnDeleteFlag) {
  rows = r;
  cols = c;

  if (!freeOnDeleteFlag) {
    elements = new vector< vector< double > > (r, vector< double>(c));
    for (int i = 0; i < r; ++i) {
      for (int j = 0; j < c; ++j) {
        elements->at(i).at(j) = el->at(i).at(j);
      }
    }
  } else {
    elements = el;
  }
}

int Matrix::getRowsCount() {
  return this->rows;
}

int Matrix::getColsCount() {
  return this->cols;
}

double Matrix::get(int i, int j) {
  assert(i < rows && i >= 0);
  assert(j < cols && j >= 0);
  return (this->elements->at(i))[j];
}

IMatrix* Matrix::set(int i, int j, double v) {
  assert(i < rows && i >= 0);
  assert(j < cols && j >= 0);
  this->elements->at(i).at(j) = v;
  return this;
}

IMatrix* Matrix::copy() {
  IMatrix* m1 = new Matrix(rows, cols, this->elements, 0);
  return m1;
}

IMatrix* Matrix::newInstance(int r, int c) {
  return new Matrix(r, c);
}

Matrix* Matrix::parseSimple(string str) {
  vector< vector< double > > *el = new vector< vector< double > >();
  int len = str.length();
  int r = 0, c = 0;
  string curr;
  double e;

  vector< double > row;

  for (int i = 0; i < len + 1; ++i) {
    if (i == len || str.at(i) == ' ') {
      if (curr.length()) {
        sscanf(curr.c_str(), "%lf", &e);
        row.push_back(e);
        curr = "";
      }
    }

    c = max(c, (int)row.size());

    if (i == len || str.at(i) == '|') {
      el->push_back(row);
      row.clear();
    } else if (str.at(i) != ' ') {
      curr = curr + str.at(i);
    }

    r = max(r, (int)el->size());
  }

  return new Matrix(r, c, el, 1);
}

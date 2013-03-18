#include "Vector.h"

#include <vector>
#include <cassert>

using namespace std;

Vector::Vector() {
  readOnly = false;
  dimension = 0;
}

Vector::~Vector() {
  delete elements;
}

Vector::Vector(vector< double > *el) {
  readOnly = false;

  dimension = el->size();

  elements = new vector< double >(dimension);
  for (int i = 0; i < dimension; ++i) {
    elements->push_back(el->at(i));
  }
}

Vector::Vector(bool readOnlyFlag, bool freeOnDeleteFlag, vector< double > *el) {
  readOnly = readOnlyFlag;

  dimension = el->size();

  if (!freeOnDeleteFlag) {
    elements = new vector< double >(dimension);
    for (int i = 0; i < dimension; ++i) {
      elements->push_back(el->at(i));
    }

  } else {
    elements = el;
  }

}

double Vector::get(int n) {
  assert(n < dimension);
  assert(n >= 0);
  return elements->at(n);
}

IVector* Vector::set(int n, double v) {
  assert(!readOnly);
  assert(n < dimension);
  assert(n >= 0);

  elements->at(n) = v;

  return this;
}

int Vector::getDimension() {
  return dimension;
}

IVector* Vector::copy() {
  return new Vector(*this);
}

IVector* Vector::newInstance(int d) {
  vector< double > *el = new vector< double >(d);
  for (int i = 0; i < d; ++i) {
    el->at(i) = 0;
  }
  return new Vector(0, 1, el);
}

Vector* Vector::parseSimple(string str) {
  vector< double > *el = new vector< double >;
  string curr;
  double e;
  int len = str.length();

  for (int i = 0; i < len + 1; ++i) {
    if (i == len || str.at(i) == ' ') {
      if (curr.length()) {
        sscanf(curr.c_str(), "%lf", &e);

        el->push_back(e);
        curr = "";
      }
    } else {
      curr = curr + str.at(i);
    }
  }

  return new Vector(0, 1, el);
}


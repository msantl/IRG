#include "AbstractVector.h"

#include "MatrixVectorView.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>

using namespace std;

AbstractVector::AbstractVector() {}

IVector* AbstractVector::copyPart(int n) {
  IVector* v1 = this->newInstance(n);

  for (int i = 0; i < min(this->getDimension(), n); ++i) {
    v1->set(i, this->get(i));
  }

  return v1;
}

IVector* AbstractVector::add(IVector* v2) {
  assert(this->getDimension() == v2->getDimension());

  for (int i = 0; i < this->getDimension(); ++i) {
    this->set(i, this->get(i) + v2->get(i));
  }

  return this;
}

IVector* AbstractVector::nAdd(IVector* v2) {
  IVector* v1 = this->copy();

  v1->add(v2);
  return v1;
}

IVector* AbstractVector::sub(IVector* v2) {
  assert(this->getDimension() == v2->getDimension());

  for (int i = 0; i < this->getDimension(); ++i) {
    this->set(i, this->get(i) - v2->get(i));
  }

  return this;
}

IVector* AbstractVector::nSub(IVector* v2) {
  IVector* v1 = this->copy();

  v1->sub(v2);
  return v1;
}

IVector* AbstractVector::scalarMultiply(double a) {
  for (int i = 0; i < this->getDimension(); ++i) {
    this->set(i, this->get(i) * a);
  }

  return this;
}

IVector* AbstractVector::nScalarMultiply(double a) {
  IVector* v1 = this->copy();

  v1->scalarMultiply(a);
  return v1;
}

double AbstractVector::norm() {
  double res = 0;
  for (int i = 0; i < this->getDimension(); ++i) {
    res += pow(this->get(i), 2);
  }
  return sqrt(res);
}

IVector* AbstractVector::normalize() {
  double norma = this->norm();

  assert(fabs(norma) > EPS); // divide by zero

  for (int i = 0; i < this->getDimension(); ++i) {
    this->set(i, this->get(i) / norma);
  }
  return this;
}

IVector* AbstractVector::nNormalize() {
  IVector* v1 = this->copy();

  v1->normalize();
  return v1;
}

double AbstractVector::cosine(IVector* v2) {
  return this->scalarProduct(v2) / (this->norm() * v2->norm());
}

double AbstractVector::scalarProduct(IVector* v2) {
  assert(this->getDimension() == v2->getDimension());

  double res = 0;
  for (int i = 0; i < this->getDimension(); ++i) {
    res += this->get(i) * v2->get(i);
  }

  return res;
}

IVector* AbstractVector::nVectorProduct(IVector* v2) {
  // ja vektorski vektore dimenzije != 3 mnoziti ne znam
  assert(this->getDimension() == 3);
  assert(this->getDimension() == v2->getDimension());

  IVector* v1 = this->copy();

  v1->set(0, this->get(1) * v2->get(2) - this->get(2) * v2->get(1));
  v1->set(1, this->get(2) * v2->get(0) - this->get(0) * v2->get(2));
  v1->set(2, this->get(0) * v2->get(1) - this->get(1) * v2->get(0));

  return v1;
}

IVector* AbstractVector::nFromHomogenus() {
  assert(this->getDimension() > 1);

  double koef = this->get(this->getDimension() - 1);
  assert(fabs(koef) > EPS); // divide by zero

  IVector* v1 = this->newInstance(this->getDimension() - 1);

  for (int i = 0; i < v1->getDimension(); ++i) {
    v1->set(i, v1->get(i) / koef);
  }

  return v1;
}

IMatrix* AbstractVector::toRowMatrix(bool isView) {
  if (isView) {
    return new MatrixVectorView(this, 1);
  } else {
    return new MatrixVectorView(this->copy(), 1);
  }
}

IMatrix* AbstractVector::toColumnMatrix(bool isView) {
  if (isView) {
    return new MatrixVectorView(this, 0);
  } else {
    return new MatrixVectorView(this->copy(), 0);
  }
}

double* AbstractVector::toArray() {
  double* res = new double[this->getDimension()];

  for (int i = 0; i < this->getDimension(); ++i) {
    res[i] = this->get(i);
  }
  return res;
}

string AbstractVector::toString() {
  string res;
  char buff[20];

  for (int i = 0; i < this->getDimension(); ++i) {
    sprintf(buff, "%3lf, ", this->get(i));

    res = res + string(buff);
  }

  return "[" + res + "]";
}

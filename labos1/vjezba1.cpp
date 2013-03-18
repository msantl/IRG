#include "Vector.h"
#include "Matrix.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  IVector* a = Vector::parseSimple("-2 4 1");
  IVector* b = a->copyPart(2);
  IVector* c = a->copyPart(5);

  cout << a->toString() << endl;
  cout << b->toString() << endl;
  cout << c->toString() << endl;

  IMatrix* m1 = Matrix::parseSimple("1 2 3 | 4 5 6 | 7 8 9");
  IMatrix* m2 = Matrix::parseSimple("9 8 7 | 6 5 4 | 3 2 1");

  cout << m1->add(m2)->toString() << endl;

  return 0;
}

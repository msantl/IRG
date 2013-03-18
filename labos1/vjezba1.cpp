#include "Vector.h"
using namespace std;

int main(int argc, char **argv) {
  IVector* a = Vector::parseSimple("-2 4 1");
  IVector* b = a->copyPart(2);
  IVector* c = a->copyPart(5);

  a->toString();
  b->toString();
  c->toString();

  return 0;
}

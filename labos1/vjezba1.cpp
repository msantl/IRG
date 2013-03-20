#include "Vector.h"
#include "Matrix.h"

#include <iostream>
#include <cassert>

using namespace std;

void zad1() {
  // izracun baricentricnih kooridnata

  IVector* a  = Vector::parseSimple("1 0 0");
  IVector* b  = Vector::parseSimple("5 0 0");
  IVector* c  = Vector::parseSimple("3 8 0");

  IVector* t  = Vector::parseSimple("3 4 0");

  double pov = b->nSub(a)->nVectorProduct(c->nSub(a))->norm() / 2.0;

  double povA = b->nSub(t)->nVectorProduct(c->nSub(t))->norm() / 2.0;
  double povB = a->nSub(t)->nVectorProduct(c->nSub(t))->norm() / 2.0;
  double povC = a->nSub(t)->nVectorProduct(b->nSub(t))->norm() / 2.0;

  cout << a->toString() << endl;
  cout << b->toString() << endl;
  cout << c->toString() << endl;
  cout << t->toString() << endl;

  double t1 = povA / pov;
  double t2 = povB / pov;
  double t3 = povC / pov;

  printf("Baricentricne koodrinate su (%.3lf %.3lf %.3lf)\n", t1, t2, t3);

  return;
}

void zad2() {
  // rjesavanje sustava jednadzbi
  IMatrix* a = Matrix::parseSimple("3 5 | 2 10");
  IMatrix* r = Matrix::parseSimple("2 | 8");

  IMatrix* v = a->nInvert()->nMultiply(r);

  printf("Rjesenje sustava je:\n%s", v->toString().c_str());

  return;
}

void zad3() {
  // baricentricne koordinate, drugi nacin
  IMatrix* a = Matrix::parseSimple("1 5 3 | 0 0 8 | 1 1 1");
  IMatrix* b = Matrix::parseSimple("3 | 4 | 1");

  IMatrix*v = a->nInvert()->nMultiply(b);

  printf("Baricentricne koordinate su:\n%s", v->toString().c_str());

  return;
}

void zad4() {
  // izracun reflektiranog vektora
  // n = (3 3), m = (2 3) => r = (3 2)
  string parse, dummy;

  getline(cin, dummy);
  printf("Unesite vektor n: \n");
  getline(cin, parse);

  IVector* n = Vector::parseSimple(parse);
  printf("Unesite vektor m: \n");
  getline(cin, parse);

  IVector* m = Vector::parseSimple(parse);

  IVector* k = n->nNormalize()->nScalarMultiply(m->scalarProduct(n) / n->norm());
  IVector* p = k->nSub(m);

  IVector* r = m->nAdd(p->nScalarMultiply(2));

  printf("Reflektirani vektor: %s\n", r->toString().c_str());

  return;
}

void demo1() {
  IVector* v1 = Vector::parseSimple("2 3 -4")->add(Vector::parseSimple("-1 4 -3"));
  double s = v1->scalarProduct(Vector::parseSimple("-1 4 -3"));
  IVector* v2 = v1->nVectorProduct(Vector::parseSimple("2 2 4"));
  IVector* v3 = v2->nNormalize();
  IVector* v4 = v2->nScalarMultiply(-1);

  IMatrix* m1 = Matrix::parseSimple("1 2 3 | 2 1 3 | 4 5 1")->add(Matrix::parseSimple("-1 2 -3 | 5 -2 7 | -4 -1 3"));
  IMatrix* m2 = Matrix::parseSimple("1 2 3 | 2 1 3 | 4 5 1")->nMultiply(Matrix::parseSimple("-1 2 -3 | 5 -2 7 | -4 -1 3")->nTranspose(1));
  IMatrix* m3 = Matrix::parseSimple("-24 18 5 | 20 -15 -4 | -5 4 1")->nInvert()->nMultiply(Matrix::parseSimple("1 2 3 | 0 1 4 | 5 6 0")->nInvert());

  cout << v1->toString() << endl;
  cout << s << endl;
  cout << v2->toString() << endl;
  cout << v3->toString() << endl;
  cout << v4->toString() << endl;

  cout << m1->toString() << endl;
  cout << m2->toString() << endl;
  cout << m3->toString() << endl;

  return;
}

void demo2() {
  double x, y, z, t;
  // 1 1 1 6
  // -1 -2 1 -2
  // 2 1 3 13

  IMatrix* a = new Matrix(3, 3);
  IMatrix* b = new Matrix(3, 1);

  for (int i = 0; i < 3; ++i) {
    scanf("%lf %lf %lf %lf", &x, &y, &z, &t);
    a->set(i, 0, x);
    a->set(i, 1, y);
    a->set(i, 2, z);
    b->set(i, 0, t);
  }

  cout << a->toString() << endl;
  cout << b->toString() << endl;

  IMatrix* r = a->nInvert()->nMultiply(b);

  cout << r->toString() << endl;

  return;
}

void demo3() {
  string dummy;
  string parseA, parseB, parseC, parseT;

  getline(cin, dummy);
  printf("Vektor A:\n");
  getline(cin, parseA);
  printf("Vektor B:\n");
  getline(cin, parseB);
  printf("Vektor C:\n");
  getline(cin, parseC);
  printf("Vektor T:\n");
  getline(cin, parseT);

  IVector* a  = Vector::parseSimple(parseA);
  IVector* b  = Vector::parseSimple(parseB);
  IVector* c  = Vector::parseSimple(parseC);

  IVector* t  = Vector::parseSimple(parseT);

  double pov = b->nSub(a)->nVectorProduct(c->nSub(a))->norm() / 2.0;

  double povA = b->nSub(t)->nVectorProduct(c->nSub(t))->norm() / 2.0;
  double povB = a->nSub(t)->nVectorProduct(c->nSub(t))->norm() / 2.0;
  double povC = a->nSub(t)->nVectorProduct(b->nSub(t))->norm() / 2.0;

  cout << a->toString() << endl;
  cout << b->toString() << endl;
  cout << c->toString() << endl;
  cout << t->toString() << endl;

  double t1 = povA / pov;
  double t2 = povB / pov;
  double t3 = povC / pov;

  printf("Baricentricne koodrinate su (%.3lf %.3lf %.3lf)\n", t1, t2, t3);

  return;
}

int main(int argc, char** argv) {
  int zad = 8;
  while(zad > 0) {
    switch(zad){
      case 1:
        zad1(); break;
      case 2:
        zad2(); break;
      case 3:
        zad3(); break;
      case 4:
        zad4(); break;
      case 5:
        demo1(); break;
      case 6:
        demo2(); break;
      case 7:
        demo3(); break;
      default:
        printf("Unesite broj [1-7] ovisno o zadatku, 0 za izlaz\n");
        break;
    }

    cin >> zad;
  }

  return 0;
}

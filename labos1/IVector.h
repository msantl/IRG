#ifndef IVECTOR_H
#define IVECTOR_H

class IVector {
  public:
    virtual double get(int);
    virtual IVector set(int, double);
    virtual int getDimension();
    virtual IVector copy();
    virtual IVector copyPart(int);
    virtual IVector newInstance(int);
    virtual IVector add(IVector);
    virtual IVector nAdd(IVector);
    virtual IVector sub(IVector);
    virtual IVector nSub(IVector);
    virtual IVector scalarMultiply(double);
    virtual IVector nScalarMultiply(double);
    virtual double norm();
    virtual IVector normalize();
    virtual IVector nNormalize();
    virtual double cosine(IVector);
    virtual double scalarProduct(IVector);
    virtual IVector nVectorProduct(IVector);
    virtual IVector nFromHomogenus();
    /*
     * virtual IMatrix toRowMatrix(bool);
     * virtual IMatrix toColumnMatrix(bool);
     */
    virtual double* toArray();

};

#endif

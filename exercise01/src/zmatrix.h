#ifndef ZMATRIX_H
#define ZMATRIX_H

#include <string>
using std::string;

class Zmatrix
{
public:
  int id, r3id, r2id, r1id;
  double l, theta, phi;
  Zmatrix(int idx, int r3idx, double lx, int r2idx, double thetax, int r1idx, double phix):
    id(idx), r3id(r3idx), r2id(r2idx), r1id(r1idx), l(lx), theta(thetax), phi(phix) { }
  // Zmatrix(){};
};

#endif

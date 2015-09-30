#ifndef ZMATRIX_H
#define ZMATRIX_H

// #include <string>
// using std::string;
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
enum ZmatrixType {cartesian, second, third, matrix};

const Vector3d DEFAULTXAXIS(0, 0, 1);

class Zmatrix
{
public:
  int id, r3id, r2id,r1id;
  double l, theta, phi;
  Vector3d coord;
  ZmatrixType type;

  Zmatrix(const int &idx, const int &r3idx, const double &lx,
          const int &r2idx, const double &thetax,
          const int &r1idx, const double &phix):
    id(idx), r3id(r3idx), r2id(r2idx),
    r1id(r1idx), l(lx),
    theta(thetax), phi(phix), type(matrix) {}

  Zmatrix(const int &idx,const double &x, const double &y, const double &z):
    id(idx), coord(x,y,z), type(cartesian){}

  Zmatrix(const int &idx, const int& r3idx, const double &lx):
    id(idx), r3id(r3idx), l(lx), type(second){}

  Zmatrix(const int &idx, const int &r3idx, const double &lx,
          const int &r2idx, const double &thetax):
    id(idx), r3id(r3idx), l(lx), r2id(r2idx), theta(thetax), type(third){}

  Zmatrix(){}

  Vector3d toCartesian(vector<Zmatrix*> &);

  void print(ostream &) const;

private:
  Vector3d getCoordinateFromAxes(const vector<Vector3d> & );
};

#endif

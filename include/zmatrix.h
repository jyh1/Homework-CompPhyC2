#ifndef ZMATRIX_H
#define ZMATRIX_H

// #include <string>
// using std::string;
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
enum ZmatrixType {cartesian, second, third, matrix};

const Vector3d DEFAULTXAXIS(1, 0, 0);

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

  // Vector3d getCartesian() const;
  // Vector3d getCartesian(const Vector3d & ) const;
  // Vector3d getCartesian(const Vector3d & , const Vector3d & ) const;
  // Vector3d getCartesian(const Vector3d & , const Vector3d & , const Vector3d &) const;

  void fromCatesian(const vector<Vector3d> &, int r1, int r2, int r3);

  void fromCatesian(const vector<Vector3d> & coords, int r2, int r3);

  void fromCatesian(const vector<Vector3d> & coords, int r3);


  void print(ostream &) const;
  void print() const;

private:
  Vector3d getCoordinateFromAxes(const vector<Vector3d> & ) const;
  void getMatrixPhi(const Vector3d & r1, const Vector3d & r2, const Vector3d & r3);
  void getMatrixTheta(const Vector3d & r2, const Vector3d & r3);
  void getMatrixL(const Vector3d & r3);
};


//
// std::vector<Vector3d> getCartesianList(const vector<Zmatrix> & ms){
//   std::vector<Vector3d> ans(ms.size());
//   auto now = ms.begin();
//   Vector3d r1 = (now++) -> getCartesian();
//   Vector3d r2 = (now++) -> getCartesian(r1);
//   Vector3d r3 = (now++) -> getCartesian(r2, r1);
//   ans.at(0) = r1;
//   ans.at(1) = r2;
//   ans.at(2) = r3;
//   for(int i = 3; i != ms.size(); i++){
//     ans.at(i) = (ms.at(i)).getCartesian(r3, r2, r1);
//     r1 = r2;
//     r2 = r3;
//     r3 = ans.at(i);
//   }
//   return ans;
// }

#endif

// #include <string>

#include <Eigen/Dense>
#include <iostream>
#include <math.h>

#include <const.h>
#include "zmatrix.h"
using std::string;

using namespace std;
using namespace Eigen;

const double Degree = 0.017453292519943295;

double vectorAngle(const Vector3d &r1,const Vector3d &r2){
  return(acos(r1.dot(r2)/r1.norm()/r2.norm()));
}

vector<Vector3d> getAxes(const Vector3d &r1,const Vector3d &r2,const Vector3d &r3)
{
  vector<Vector3d> l0;
  Vector3d i = (r3 - r2).normalized(), r21 = r1 - r2;
  Vector3d e = (r21 - (r21.dot(i) * i)).normalized();
  l0.push_back(i); l0.push_back(e); l0.push_back(i.cross(e));
  return l0;
}


void Zmatrix::print(ostream &output) const {
  output << id << ' ' << 'A' << id << ' ';

  switch (type) {
    case cartesian:
      output << coord[0] << ' ' << coord[1] << ' ' << coord[2];
      break;
    case matrix:
      output << r3id << ' ' <<
        l << ' ' << r2id <<  ' '
        << theta / Degree<< ' ' << r1id << ' ' << phi / Degree;
      break;
    case second:
      output << r3id << ' ' << l;
      break;
    case third:
      output << r3id << ' ' << l << ' ' << r2id << ' ' << theta / Degree;
      break;
  }
  output << endl;
}

void Zmatrix::print() const{
  print(cout);
}


Vector3d Zmatrix::toCartesian(vector<Zmatrix*> &points) {
  switch (type) {
    case second:
      coord = (points[r3id]->toCartesian(points)) + l * DEFAULTXAXIS;
      break;

    case third:{
      Vector3d r3coord = points[r3id]->toCartesian(points);
      Vector3d r2coord = points[r2id]->toCartesian(points);
      Matrix3d rotate;
      rotate << cos(theta), sin(theta), 0,
                -sin(theta), cos(theta), 0,
                0, 0, 1;
      coord = ((rotate * (r2coord - r3coord)).normalized() * l + r3coord);
      break;}

      case matrix:{
        Vector3d r3coord = points[r3id]->toCartesian(points);
        Vector3d r2coord = points[r2id]->toCartesian(points);
        Vector3d r1coord = points[r1id]->toCartesian(points);
        coord = r3coord + getCoordinateFromAxes(getAxes(r1coord, r2coord, r3coord));
        break;}

    }
  type = cartesian;
  return coord;
}

Vector3d Zmatrix::getCoordinateFromAxes(const vector<Vector3d> & axes)const{
  auto &i = axes.at(0);
  auto &e = axes.at(1);
  auto &k = axes.at(2);
  return l * (sin(theta)*cos(phi) * e -
                  sin(theta)*sin(phi) * k - cos(theta) * i);
}

void Zmatrix::fromCatesian(const vector<Vector3d> & coords,
                                int r1, int r2, int r3){
  if(type != cartesian){
    cerr << "Error in cartesian2Matrix!" << endl;
    return;
  }
  type = matrix;
  r3id = r3; r2id = r2;r1id = r1;
  getMatrixPhi(coords.at(r1), coords.at(r2), coords.at(r3));
  getMatrixTheta(coords.at(r2), coords.at(r3));
  getMatrixL(coords.at(r3));
}

void Zmatrix::fromCatesian(const vector<Vector3d> & coords,
                                  int r2, int r3){
  if(type != cartesian){
    cerr << "Error in catesian2Third!" << endl;
    return;
  }
  type = third;
  r3id = r3; r2id = r2;
  getMatrixTheta(coords.at(r2), coords.at(r3));
  getMatrixL(coords.at(r3));
}

void Zmatrix::fromCatesian(const vector<Vector3d> & coords,
                                  int r3){
  if(type != cartesian){
    cerr << "Error in catesian2Second!" << endl;
    return;
  }
  type = second;
  r3id = r3;
  getMatrixL(coords.at(r3));
}


void Zmatrix::getMatrixPhi(const Vector3d & r1, const Vector3d & r2, const Vector3d & r3){
  Vector3d r21 = r1 - r2, r23 = r3 - r2,
                  r34 = coord - r3;
  auto r2123 = r21.cross(r23);
  auto r2334 = - r23.cross(r34);
  phi = vectorAngle(r2123, r2334);

  if(r2123.cross(r2334).dot(r21) > 0){
    phi = 2 * pi - phi;
  }
}

void Zmatrix::getMatrixTheta(const Vector3d & r2, const Vector3d & r3){
  Vector3d r23 = r3 - r2,
            r34 = coord - r3;
  theta = vectorAngle(r34,-r23);
}

void Zmatrix::getMatrixL(const Vector3d & r3){
  l = (coord - r3).norm();
}

Vector3d Zmatrix::getCartesian() const{
  return coord;
}
Vector3d Zmatrix::getCartesian(const Vector3d & r3)const{
  return r3 + l * DEFAULTXAXIS;
}
Vector3d Zmatrix::getCartesian(const Vector3d &r3, const Vector3d & r2) const{
  Matrix3d rotate;
  rotate << cos(theta), sin(theta), 0,
            -sin(theta), cos(theta), 0,
            0, 0, 1;
  return ((rotate * (r2 - r3)).normalized() * l + r3);
}
Vector3d Zmatrix::getCartesian(const Vector3d &r3, const Vector3d &r2,
                                const Vector3d &r1)const {
  return r3 + getCoordinateFromAxes(getAxes(r1, r2, r3));
}





//ex2test
// int main(int argc, char const *argv[]) {
//   Vector3d v1(0.0, 0.0, 0),
//             v2(1.5,0,0),v3(2.00071,1.41396,0);
//   Zmatrix s(1,3.50071,1.41396,0);
//   // s.cartesian2Matrix(v1,v2,v3);
//   s.print();
//   cout << vectorAngle(v1,v2) << endl;
//   return 0;
// }

//ex1test
// int main()
// {
//   // Zmatrix s(4, 3, 1.5, 2, 1.91114, 1, 3.1415826);
//   Zmatrix s(3, 2, 1.5, 1, 1.91114);
//   s.print(cout);
//   std::vector<Zmatrix*> v = {new Zmatrix(),new Zmatrix(1, 0, 0, 0)
//                             , new Zmatrix(2, 1.5, 0, 0),
//                               new Zmatrix(3, 2.00071, 1.41396,0)};
//   cout << s.toCartesian(v) << endl;
//   return 0;
// }

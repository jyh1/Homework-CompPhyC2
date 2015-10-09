// #include <string>

#include <Eigen/Dense>
#include <iostream>
#include <math.h>
#include "zmatrix.h"
using std::string;

using namespace std;
using namespace Eigen;


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
        << theta << ' ' << r1id << ' ' << phi;
      break;
    case second:
      output << r3id << ' ' << l;
      break;
    case third:
      output << r3id << ' ' << l << ' ' << r2id << ' ' << theta;
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

Vector3d Zmatrix::getCoordinateFromAxes(const vector<Vector3d> & axes){
  auto &i = axes.at(0);
  auto &e = axes.at(1);
  auto &k = axes.at(2);
  return l * (sin(theta)*cos(phi) * e -
                  sin(theta)*sin(phi) * k - cos(theta) * i);
}

void Zmatrix::cartesian2Matrix(const vector<Vector3d> & coords,
                                int r1, int r2, int r3){
  type = matrix;
  r3id = r3; r2id = r2;r1id = r1;
  getMatrixAngle(coords.at(r1), coords.at(r2), coords.at(r3));
}

void Zmatrix::getMatrixAngle(const Vector3d & r1, const Vector3d & r2, const Vector3d & r3){
  if(type != cartesian){
    cerr << "Error in cartesian2Matrix!" << endl;
    return;
  }
  Vector3d r21 = r1 - r2, r23 = r3 - r2,
                  r34 = coord - r3;
  l = r34.norm();
  theta = vectorAngle(r34,-r23);
  phi = vectorAngle(r21.cross(r23),- r23.cross(r34));
}

//ex2test
int main(int argc, char const *argv[]) {
  Vector3d v1(0.0, 0.0, 0),
            v2(1.5,0,0),v3(2.00071,1.41396,0);
  Zmatrix s(1,3.50071,1.41396,0);
  // s.cartesian2Matrix(v1,v2,v3);
  s.print();
  cout << vectorAngle(v1,v2) << endl;
  return 0;
}

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
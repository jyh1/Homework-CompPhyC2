// #include <string>

#include <Eigen/Dense>
#include <iostream>
#include <math.h>
#include "zmatrix.h"
using std::string;

using namespace std;
using namespace Eigen;


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

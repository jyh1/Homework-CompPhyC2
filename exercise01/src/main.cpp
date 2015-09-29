#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <math.h>
#include "zmatrix.h"

using namespace Eigen;
using namespace std;



vector<Vector3d> getAxes(const Vector3d &r1,const Vector3d &r2,const Vector3d &r3)
{
  vector<Vector3d> l0;
  Vector3d i = (r3 - r2).normalized(), r21 = r1 - r2;
  Vector3d e = (r21 - (r21.dot(i) * i)).normalized();
  l0.push_back(i); l0.push_back(e); l0.push_back(i.cross(e));
  return l0;
}

Vector3d getCoordinate(const vector<Vector3d> &axes, const Vector3d &r3,const double &l, const double &theta, const double &phi)
{
  auto &i = axes.at(0);
  auto &e = axes.at(1);
  auto &k = axes.at(2);
  return r3 + l * (sin(theta)*cos(phi) * e - sin(theta)*sin(phi) * k - cos(theta) * i);
}

Vector3d getFourthCoordinate(const Vector3d &r1, const Vector3d &r2,const Vector3d &r3, const double &l, const double &theta, const double &phi)
{
  return getCoordinate(getAxes(r1, r2, r3), r3, l, theta, phi);
}



int main(){
  Vector3d v1 = Vector3d(0,0,0);
  Vector3d v2 = Vector3d(1.5,0,0);
  Vector3d v3 = Vector3d(2.00071,1.41396,0);
  vector<Vector3d> l = getAxes(v1, v2, v3);
  cout << getFourthCoordinate(v1, v2, v3, 1.5, 1.911135530933791, 3.14159265358) << endl;
  // l.push_back(v1);l.push_back(v2);l.push_back(v3);
  // cout << (l.front()).matrix().transpose() << endl;
  // for (auto i = l.begin(); i != l.end(); ++i){
  //   cout << *i << endl;
  // }
}

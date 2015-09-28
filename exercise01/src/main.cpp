#include <iostream>
#include <Eigen/Dense>
#include <list>

using namespace Eigen;
using namespace std;

// list<Vector3d>

int main(){
  Vector3d v1 = Vector3d(1,2,3);
  Vector3d v2 = Vector3d(4,5,6);
  Vector3d v3 = Vector3d(7,8,9);
  list<Vector3d> l;
  l.push_back(v1);l.push_back(v2);l.push_back(v3);
  // cout << (l.front()).matrix().transpose() << endl;
  for (auto i = l.begin(); i != l.end(); ++i){
    cout << *i << endl;
  }
}

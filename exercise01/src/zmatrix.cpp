#include <string>

#include <Eigen/Dense>
using std::string;
#include "iostream"
using namespace std;

using namespace Eigen;

enum ZmatrixType {cartesian, second, third, matrix};

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

  void print(ostream &);
  // Zmatrix(){};
};

void Zmatrix::print(ostream &output){
  output << id << ' ' << 'A' << id << ' ';

  switch (type) {
    case cartesian:
      output << coord.matrix().transpose();
      break;
    case matrix:
      output << r3id << ' ' <<
        l << ' ' << r2id <<  ' '
        << theta << ' ' << r1id << ' ' << phi << ' ';
      break;
    case second:
      output << r3id << ' ' << l << ' ';
      break;
    case third:
      output << r3id << ' ' << l << ' ' << r2id << ' ' << theta;

  }
  output << endl;
  // TODO
}

int main()
{
  Zmatrix s(1,2,3,4,5);
  s.print(cout);
  return 0;
}

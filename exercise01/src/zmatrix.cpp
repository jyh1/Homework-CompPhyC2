#include <string>

#include <Eigen/Dense>
#include <iostream>
#include "zmatrix.h"
using std::string;

using namespace std;
using namespace Eigen;

void Zmatrix::print(ostream &output){
  output << id << ' ' << 'A' << id << ' ';

  switch (type) {
    case cartesian:
      output << coord.matrix().transpose();
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



// int main()
// {
//   Zmatrix s(1,2,3,4,5);
//   s.print(cout);
//   return 0;
// }

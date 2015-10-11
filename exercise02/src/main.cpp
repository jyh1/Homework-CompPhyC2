#include <iostream>
#include <vector>
#include <math.h>
#include "zmatrix.h"
#include "parsezmatrix.h"
#include <fstream>

using namespace Eigen;
using namespace std;

int main(int argc, char const *argv[]) {
  fstream ifs;
  ifs.open ("data.dat", std::ifstream::in);
  list<Zmatrix*> matrixes;
  makeZmatrixFileParser(matrixes)(ifs);

  vector<Vector3d> coord(matrixes.size() + 2);
  for (auto i:matrixes){
    coord.at(i->id) = i -> coord;
    // cout << i->type << endl;
  }
  for(auto i:matrixes){
    if((i->id) > 3){
      i->fromCatesian(coord, i->id -3, i->id -2, i -> id - 1);
    }
  }
  auto origin = matrixes.begin();
  (*(++origin))->fromCatesian(coord, 1);
  (*(++origin))->fromCatesian(coord, 1, 2);

  for(auto i:matrixes){
    i -> print();
  }
  return 0;
}

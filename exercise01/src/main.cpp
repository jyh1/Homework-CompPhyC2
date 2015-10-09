#include <iostream>
#include <vector>
#include <math.h>
#include "zmatrix.h"
#include "parsezmatrix.h"
#include <fstream>

using namespace Eigen;
using namespace std;


int main(){
    fstream ifs;
    ifs.open ("../../data/zmatrix_in.dat", std::ifstream::in);
    list<Zmatrix*> matrixes;
    makeZmatrixFileParser(matrixes)(ifs);
    std::vector<Zmatrix*> vmatrix(12);
    for (auto i = matrixes.begin(); i != matrixes.end(); i++){
      vmatrix.at((*i)->id) = *i;
    }

    for (auto i = matrixes.begin(); i!= matrixes.end(); i++){
      (*i)->toCartesian(vmatrix);
      (*i) -> print(cout);

    }
}

#include <iostream>
#include <vector>
#include <math.h>
#include "zmatrix.h"
#include "parsezmatrix.h"
#include <fstream>

using namespace Eigen;
using namespace std;


int main(int argv, char** argc){
    fstream ifs;
    if(argv == 1){
      ifs.open ("../../data/zmatrix_in.dat", std::ifstream::in);
    }else{
      ifs.open(argc[1], std::ifstream::in);
    }
    list<Zmatrix*> matrixes;
    makeZmatrixFileParser(matrixes)(ifs);
    std::vector<Zmatrix*> vmatrix(matrixes.size() + 2);
    for (auto i = matrixes.begin(); i != matrixes.end(); i++){
      vmatrix.at((*i)->id) = *i;
    }

    for (auto i = matrixes.begin(); i!= matrixes.end(); i++){
      (*i)->toCartesian(vmatrix);
      (*i) -> print(cout);

    }
}

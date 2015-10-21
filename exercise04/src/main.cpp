#include <iostream>
#include <fstream>
#include <functional>


#include <random.h>
#include <parse.h>
#include <parsezmatrix.h>
#include <montecarlo.h>

int main(int argc, char const *argv[]) {
  std:: fstream ifs;

  if(argc == 1){
    ifs.open("../../data/phisample_in.dat", std::ifstream::in);
  }else{
    ifs.open(argv[1], std::ifstream::in);
  }

  double l, theta, kbt, kphi;
  makeParametersParser(l, theta, kbt, kphi)(ifs);//defined in pasezmatrix.h

  int L, M;
  Parser req = makeRequirementsParser(L, M);//defined in parsezmatrix.h

  while (req(ifs) == ParseSucceed){
    monteCarloPhiSample(L, M, kbt, kphi, getUniformRandomPhi);//defined in montecarlo.h
  }

  return 0;
}

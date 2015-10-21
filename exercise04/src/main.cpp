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
  list<Parser> ps = {deleteComments,
                    makeDoubleParser(l),
                    makeAngleParser(theta),
                    makeDoubleParser(kbt),
                    makeDoubleParser(kphi),
                  };
  parseSerial(ps)(ifs);

  int L, M;
  std::list<Parser> req = {deleteComments,
                      makeIntParser(L), makeIntParser(M)};
  while (parseSerial(req)(ifs) == ParseSucceed){
    monteCarloPhiSample(L, M, kbt, kphi, getUniformRandomPhi);
  }

  return 0;
}

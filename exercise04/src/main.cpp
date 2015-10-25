#include <iostream>
#include <fstream>
#include <functional>


#include <random.h>
#include <parse.h>
#include <parsezmatrix.h>
#include <montecarlo.h>

class Simulationt4 : public Simulationfort4t5{

public:
  Simulationt4(double kbt,double kphi)
    :Simulationfort4t5(getUniformRandomPhi,kbt, kphi){}

private:
  void consume(const double & phi){
    integrateQTemp += bolzman(phi);
    integrateEnergyBolzTemp += energy(phi) * bolzman(phi);
    // std::cout << integrateQTemp << ' ' << ' ' << integrateEnergyBolzTemp << std::endl;
  }
  double getPartition()const{
    return (2 * pi * integrateQTemp / getSimulationTimes());
  }
  double getInternal()const{
    return (integrateEnergyBolzTemp / integrateQTemp);
  }
};

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
  Simulationt4 simulation(kbt, kphi);

  while (req(ifs) == ParseSucceed){
    monteCarlo(L, M, simulation);//defined in montecarlo.h
  }

  return 0;
}

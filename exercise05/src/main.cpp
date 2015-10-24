#include <iostream>
#include <fstream>
#include <functional>


#include <random.h>
#include <parse.h>
#include <parsezmatrix.h>
#include <montecarlo.h>

class Simulationt5 : public Simulationfort4t5{

public:
  Simulationt5(double kbt,double kphi)
    :Simulationfort4t5(std::bind(getBoltzmanPhi, kbt, kphi),
                        kbt, kphi){}

private:
  void consume(const double & phi){
    integrateQTemp += exp(energy(phi)/kbt);
    integrateEnergyBolzTemp += energy(phi);
  }
  double getPartition(){
    return 1 / integrateQTemp * getSimulationTimes() * 2 * pi;
  }
  double getInternal(){
    return integrateEnergyBolzTemp / getSimulationTimes();
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
  Simulationt5 simulation(kbt, kphi);

  while (req(ifs) == ParseSucceed){
    monteCarlo(L, M, simulation);//defined in montecarlo.h
  }

  return 0;
}

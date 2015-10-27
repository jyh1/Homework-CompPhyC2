
// #define DEBUG

#include <iostream>
#include <functional>
#include <fstream>

// #include <string>

#include <montecarlo.h>
#include <random.h>
#include <parse.h>
#include <parsezmatrix.h>

class Simulationfort6 : public Simulationfort6t7{
public:
  Simulationfort6(const int & n,
                    const double &bx,const double &thetax,
                    const double &kbtx,
                    const double &kphix, const double& epsinonx,
                    const double &sigmax, const double& alphax):
                    Simulationfort6t7(n, bx, thetax, kbtx, kphix, epsinonx,
                                      sigmax, alphax,
                                      std::bind(getBoltzmanPhi, kbtx, kphix)){}
protected:
  void makeNewPolymer(){
    for(int j = 3; j != chainL; j++){
      double phi = generator();
      growAt(j, phi);
      energyT += energyt(phi);
      #ifdef DEBUG
      std::cout << phi << std::endl;
      #endif
    }
  }

protected:
  void simulateN(const int &n){
    for(int i =0; i!=n; i++){
      energyT = 0;
      makeNewPolymer();
      updateSum();
    }
    addSimulationTimes(n);
  }

  void updateSum(){
    double ev = totalEnergyv();
    double et = totalEnergyt();
    wn += bolzmanCof(ev);
    partition += 1/bolzmanCof(et);
    internal += (ev + et)*bolzmanCof(ev);
    sumGyration += gyration()*bolzmanCof(ev);
    #ifdef DEBUG
    std::cout << ev << ' ' << et << std::endl;
    std::cout << sum1 << ' '<< sum2 << ' ' << sum3 << "sum" << std::endl;
    #endif
  }
};

int main(int argc, char const *argv[]) {
  std:: fstream ifs;

  if(argc == 1){
    ifs.open("../../data/IRS_polymer_MC_in.dat", std::ifstream::in);
  }else{
    ifs.open(argv[1], std::ifstream::in);
  }
  int cl;
  double b, theta ,kbt, kphi, epsinon, sigma, alpha;

  std::list<Parser> p = {
                        makeIntParser(cl),
                        makeDoubleParser(b),
                        makeAngleParser(theta),
                        makeDoubleParser(kbt),
                        makeDoubleParser(kphi),
                        makeDoubleParser(epsinon),
                        makeDoubleParser(sigma),
                        makeDoubleParser(alpha),deleteComments};

  parseSerial(p, deleteComments)(ifs);

#ifndef DEBUG
  Simulationfort6 simulation(cl, b, theta, kbt, kphi,epsinon, sigma, alpha);
#endif

#ifdef DEBUG
  std::cout << kbt << std::endl;
  Simulationfort6 simulation(5, b, theta, kbt, kphi,epsinon, sigma, alpha);
#endif

  int L, M;
  Parser req = makeRequirementsParser(L, M);
#ifndef DEBUG
  while (req(ifs) == ParseSucceed){
    monteCarlo(L, M, simulation);
  }
#endif
#ifdef DEBUG
  monteCarlo(1,1,simulation);
#endif

  return 0;
}

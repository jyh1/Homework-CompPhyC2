// #define DEBUG
#include <iostream>
#include <functional>
#include <fstream>


#include <montecarlo.h>
#include <random.h>
#include <parse.h>
#include <parsezmatrix.h>

class Simulationfort7 : public Simulationfort6t7{
private:
  // const double Wupper = 3000000000, Wlower = -0.03;
  const double Wupper = 0.3, Wlower = 0.03;
  const double delta = 0.000001;
  // double Cupper, Clower;
public:
  Simulationfort7(const int & n,
                    const double &bx,const double &thetax,
                    const double &kbtx,
                    const double &kphix, const double& epsinonx,
                    const double &sigmax):
                    Simulationfort6t7(n, bx, thetax, kbtx, kphix, epsinonx,
                                      sigmax, 0,//set alpha 0 to eliminate soft truntion
                                      std::bind(getBoltzmanPhi, kbtx, kphix)){}
private:
  void step(const int i,const double W,const double et){
    if(i >= chainL){
      energyT = et;
      updateSum(W,et);
      addSimulationTimes(1);
      return;
    }
    double phi = generator();
    growAt(i,phi);
    double etNow = et + energyt(phi);

    double wnow = W * bolzmanCof(energyvAt(i));


    if(wnow < delta) return;//too unlikely ,discard

    if(wnow > Wupper){
      step(i+1, wnow / 2, etNow);
      step(i+1, wnow/2, etNow);
      return;
    }
    if(wnow < Wlower){
      if(unitRandom() < 0.5){
        step(i + 1, wnow * 2, etNow);
      }
      return;
    }
    step(i+1,wnow, etNow);
  }

  void updateSum(const double &w, const double &et){
    double ev = totalEnergyv();
    wn += w;
    partition += 1/bolzmanCof(ev+et) * w;
    internal += (ev+et) * w;
    sumGyration += gyration() * w;
  }


protected:
  void simulateN(const int &n){
    while(getSimulationTimes() < n){
      step(3, 1, 0);
      // cout << getSimulationTimes()<<endl;

    }
  }
};




int main(int argc, char const *argv[]) {
  std:: fstream ifs;

  if(argc == 1){
    ifs.open("../../data/polymer_MC_in.dat", std::ifstream::in);
  }else{
    ifs.open(argv[1], std::ifstream::in);
  }

  int cl;
  double b, theta ,kbt, kphi, epsinon, sigma;

  std::list<Parser> p = {
                        makeIntParser(cl),
                        makeDoubleParser(b),
                        makeAngleParser(theta),
                        makeDoubleParser(kbt),
                        makeDoubleParser(kphi),
                        makeDoubleParser(epsinon),
                        makeDoubleParser(sigma),
                        deleteComments};
  parseSerial(p, deleteComments)(ifs);

// #ifndef DEBUG
  Simulationfort7 simulation(cl, b, theta, kbt, kphi,epsinon, sigma);
// #endif
// #ifdef DEBUG
  std::cout << kbt << std::endl;
  Simulationfort7 simulation(6, b, theta, kbt, kphi,epsinon, sigma);
// #endif

  int L, M;
  Parser req = makeRequirementsParser(L, M);
// #ifndef DEBUG
  while (req(ifs) == ParseSucceed){
    monteCarlo(L, M, simulation);
  }
// #endif
// #ifdef DEBUG
//   monteCarlo(1,1,simulation);
// #endif

  return 0;
}

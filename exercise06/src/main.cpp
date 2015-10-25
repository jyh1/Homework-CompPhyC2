
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
private:
  double sum1 = 0, sum2 = 0, sum3 = 0;
public:
  Simulationfort6(const int & n,
                    const double &bx,const double &thetax,
                    const double &kbtx,
                    const double &kphix, const double& epsinonx,
                    const double &sigmax, const double& alphax):
                    Simulationfort6t7(n, bx, thetax, kbtx, kphix, epsinonx,
                                      sigmax, alphax,
                                      std::bind(getBoltzmanPhi, kbtx, kphix)){}
private:
  void resetOthers(){
    sum1 = 0;sum2=0;sum3=0;
  }

  void makeNewPolymer(){
    for(int j = 3; j != chainL; j++){
      Vector3d r3 = xyz.at(j - 1),
                r2 = xyz.at(j - 2),
                r1 = xyz.at(j - 3);
      Vector3d i = (r3 - r2).normalized(), r21 = r1 - r2;
      Vector3d e = (r21 - (r21.dot(i) * i)).normalized();
      Vector3d k = i.cross(e);
      double phi = generator();
      xyz.at(j) = r3 + b * (sin(theta)*cos(phi) * e -
                      sin(theta)*sin(phi) * k - cos(theta) * i);
      energyT += energyt(phi);
      #ifdef DEBUG
      std::cout << phi << std::endl;
      #endif
    }
  }

  void updateSum(){
    double ev = totalEnergyv();
    double et = totalEnergyt();
    sum1 += bolzmanCof(ev);
    sum2 += 1 / bolzmanCof(et);
    sum3 += (ev + et)*bolzmanCof(ev);
    #ifdef DEBUG
    std::cout << ev << ' ' << et << std::endl;
    std::cout << sum1 << ' '<< sum2 << ' ' << sum3 << "sum" << std::endl;
    #endif

  }

  double getPartition() const{
    return pow(2 * pi,chainL - 3) *
          sum1 / sum2;
  }
  double getInternal() const{
    return sum3 / sum1;
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

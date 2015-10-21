#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <functional>
#include <list>
#include <iostream>
#include <string>
#include <algorithm>


#include <const.h>
#include <statistics.h>

typedef std::function<double()> RandomGenerator;
typedef std::function<double(double)> Func;
typedef std::function<std::list<double>(const std::list<double> &)> Test;

void monteCarlo(const int &L, const int& M,
                const RandomGenerator & rand,
                const Test & tests,
                const std::list<std::string> & names,
                std::ostream& output = std::cout){
  std::list<std::list<double>> res;
  for(int i = 0; i != M; i++){
    std::list<double> xs;
    for(int j = 0; j != L; j++){
      xs.push_back(rand());//random generator from argument
    }

    res.push_back(tests(xs));

  }

  output<<"# number-of-conformers-to-sample(L) number-of-repetitions(M)"
        <<std::endl;
  output << L << ' ' << M << std::endl;

  output<<"# quantity-names ensamble-average error-range" << std::endl;

  auto nameP = names.begin();
  while(! ( (res.begin())->empty() ) ){
    std::list<double> resn;
    for_each(res.begin(), res.end(),
            [&](std::list<double> & col){
              resn.push_back(col.front());
              col.pop_front();
            }
    );

    output << *nameP <<' '
            << meanBy(resn) << ' '
            << standardDeviation(resn) << std::endl;
    nameP++;
  }
}


//t4 t5 simulation
void monteCarloPhiSample(const int& L, const int& M,
                        const double& kbt,
                        const double& kphi,
                        const RandomGenerator & rand,
                        // const Func & energy,
                        std::ostream & ouptut = std::cout){

  const Func energy = [&](const double &phi){
                        return((1+cos(3 * phi)) * kphi / 2);
                      };
  const Func bolzmanCof = [&](const double & phi){
                            return (exp(-energy(phi)/kbt));
                          };
  const Func energyProb = [&](const double &phi){
                            return (energy(phi) * bolzmanCof(phi));
                          };

  const Test test = [&](const std::list<double> & phis){
                            double qTemp = totalBy(phis, bolzmanCof);
                            double q = 2 * pi * qTemp / phis.size();//partition function
                            double U = totalBy(phis, energyProb) / qTemp;
                            double helmF = - kbt * log(q);
                            double entropy = (U - helmF)/kbt * kbt;
                            std::list<double> ans = {helmF, U, entropy};
                            return ans;
                          };
  std::list<std::string> names = {"Helmholtz-free-energy",
                                "Internal-energy","Entropy"};
  monteCarlo(L, M, rand, test,names);
}

#endif

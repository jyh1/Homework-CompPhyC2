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

class Simulation {
private:
  RandomGenerator generator;
  int N=0;
protected:
  std::list<std::string> quantityNames;
public:
  Simulation (RandomGenerator g){
    generator = g;
  }
  virtual ~Simulation (){}

private:
  void simulateN(const int & n){
    for(int i = 0; i != n ; i++){
      consume(generator());
    }
    N += n;
  }

public:
  std::list<double> runSimulation(const int & n){
    simulateN(n);
    auto ans = getResults();
    reset();
    return ans;
  }

  const std::list<std::string> & getQuantityNames() const{
    return quantityNames;
  }
protected:
  int getSimulationTimes() const {
    return N;
  }

  void reset(){
    N = 0;
    resetOthers();
  }

protected:
  virtual void consume(const double &){
    std::cerr << "undefined consume function" << std::endl;
  }

  virtual void resetOthers(){
    std::cerr << "undefined resetOthers" << std::endl;
  }

  virtual std::list<double> getResults(){
    std::list<double> ans;
    std::cerr << "undefined getResults" << std::endl;
    return ans;
  }
};


class Simulationfort4t5 : public Simulation{
protected:
  double integrateQTemp = 0;
  double integrateEnergyBolzTemp = 0;
protected:
  double kbt;
  double kphi;
protected:
  double energy(const double & phi)const{
    return 0.5 * kphi * (1 + cos(3 * phi));
  }
  double bolzman(const double & phi)const{
    return exp(- energy(phi)/kbt);
  }
  virtual double getPartition(){
    std::cerr << "undefined partition function" << std::endl;
  }
  virtual double getInternal(){
    std::cerr << "undefined getInternal" << std::endl;
  }
public:
  Simulationfort4t5(RandomGenerator g, double kbt1, double kphi1)
    :Simulation(g),kbt(kbt1),kphi(kphi1){
    quantityNames = {"Helmholtz-free-energy",
                    "Internal-energy","Entropy"};
  }

  std::list<double> getResults(){
    double q = getPartition();
    double U = getInternal();
    double helmF = - kbt * log(q);
    double entropy = (U - helmF) / kbt;
    std::list<double> ans = {helmF, U, entropy};
    // std::cout << helmF << ' ' << U << ' '<<q<<' ' << entropy << std::endl;
    return ans;
  }
private:
  void resetOthers(){
    integrateQTemp = 0;
    integrateEnergyBolzTemp = 0;
  }

};



void monteCarlo(const int &L, const int& M,
                Simulation & simulation,
                std::ostream& output = std::cout){
  std::list<std::list<double>> res;
  for(int i = 0; i != M; i++){
    res.push_back(simulation.runSimulation(L));
  }

  output<<"# number-of-conformers-to-sample(L) number-of-repetitions(M)"
        <<std::endl;
  output << L << ' ' << M << std::endl;

  output<<"# quantity-names ensamble-average error-range" << std::endl;

  auto names = simulation.getQuantityNames();

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



#endif

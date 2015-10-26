#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <functional>
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>
#include <Eigen/Dense>


#include <const.h>
#include <statistics.h>
// #include <zmatrix.h>

using namespace Eigen;
typedef std::function<double()> RandomGenerator;
typedef std::function<double(double)> Func;
typedef std::function<std::list<double>(const std::list<double> &)> Test;

class Simulation {
private:
  int N=0;
protected:
  RandomGenerator generator;
  std::list<std::string> quantityNames;
  double kbt;
public:
  Simulation (RandomGenerator g, const double & kbtx):kbt(kbtx){
    generator = g;
    #ifdef DEBUG
    std::cout << kbt << std::endl;
    #endif
  }
  virtual ~Simulation (){}

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

  void addSimulationTimes(const double &d) {
    N += d;
  }

  void reset(){
    N = 0;
    resetOthers();
  }

protected:
  double bolzmanCof(const double & e)const{
    return exp(- e / kbt);
  }

protected:
  //functions which must be provided by derived class
  virtual void update(){
    std::cerr << "undefined update function" << std::endl;
  }

  virtual void resetOthers(){
    std::cerr << "undefined resetOthers" << std::endl;
  }

  virtual std::list<double> getResults()const{
    std::list<double> ans;
    std::cerr << "undefined getResults" << std::endl;
    return ans;
  }
  virtual double getPartition()const{
    std::cerr << "undefined partition function" << std::endl;
  }
  virtual double getInternal()const{
    std::cerr << "undefined getInternal" << std::endl;
  }

  virtual void simulateN(const int & n){
    std::cout << "undefined simulateN" << std::endl;
  }


};




class Simulationfort4t5 : public Simulation{
protected:
  double integrateQTemp = 0;
  double integrateEnergyBolzTemp = 0;
protected:
  double kphi;
protected:
  double energy(const double & phi)const{
    return 0.5 * kphi * (1 + cos(3 * phi));
  }
  double bolzman(const double & phi)const{
    return exp(- energy(phi)/kbt);
  }

public:
  Simulationfort4t5(RandomGenerator g, double kbt1, double kphi1)
    :Simulation(g,kbt1),kphi(kphi1){
    quantityNames = {"Helmholtz-free-energy",
                    "Internal-energy","Entropy"};
  }

  std::list<double> getResults()const{
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
protected:
  void update(){
    consume(generator());
  }

  void simulateN(const int &n){
    for(int i = 0 ; i != n; i++){
      update();
    }
    addSimulationTimes(n);
  }

  virtual void consume(const double & phi){
    std::cerr << "undefiend consume function" << std::endl;
  }

};


class Simulationfort6t7 : public Simulation{
private:
  double kphi;
protected:
    double sum1 = 0, sum2 = 0, sum3 = 0,sumGyration=0;

protected:
  double energyT = 0;
  std::vector<Vector3d> xyz;
  double epsinon,alphasigma,sigma,b,theta;
  int chainL;
protected:
  double energyt(const double & phi)const{
    return 0.5 * kphi * (1 + cos(3 * phi));
  }

  double hardEnergyv(const double &d)const{
    #ifdef DEBUG
    std::cout << "hardEnergyv" << 4 * epsinon * (pow(sigma/d, 12) - pow(sigma/d,6))
      << std::endl;
    #endif
    return 4 * epsinon * (pow(sigma/d, 12) - pow(sigma/d,6));
  }

  double energyv(const double & d)const{
    if (d < alphasigma) {
      return hardEnergyv(alphasigma);
    }
    else {
      return hardEnergyv(d);
    }
  }

  double totalEnergyt()const{
    return energyT;
  }

  double totalEnergyv()const{
    double ener = 0;
    for(auto i = xyz.begin() + 4; i != xyz.end(); i++){
      for(auto j = xyz.begin(); j != (i - 3); j++){
        ener += energyv((*i - *j).norm());
      }
    }
    return ener;
  }

  double energyvAt(const int & i) const{
    double ener = 0;
    for(int j = 0; j <= i - 4; j++){
      ener += energyv((xyz.at(j) - xyz.at(i)).norm());
    }
    return ener;
  }

  double energy()const{
    return (totalEnergyv() + totalEnergyt());
  }


  double gyration() const{
    Vector3d m(0,0,0);
    for(auto i:xyz){
      m += i;
    }
    m /= xyz.size();
    double ans = 0;
    for(auto i:xyz){
      ans += (i - m).squaredNorm();
    }
    return (ans / xyz.size());
  }

public:
  Simulationfort6t7(const int & n,
                    const double &bx,const double &thetax,
                    const double &kbtx,
                    const double &kphix, const double& epsinonx,
                    const double &sigmax, const double& alphax,
                    RandomGenerator g):
                    chainL(n),b(bx),theta(thetax),
                    kphi(kphix),
                    epsinon(epsinonx),sigma(sigmax),alphasigma(alphax * sigmax),
                    Simulation(g, kbtx){
  xyz.resize(n);
  xyz.at(0)=Vector3d(0,0,0);
  xyz.at(1) = Vector3d(bx, 0, 0);

  Matrix3d rotate;
  rotate << cos(theta), sin(theta), 0,
            -sin(theta), cos(theta), 0,
            0, 0, 1;
  xyz.at(2) = (rotate * Vector3d(-bx,0,0)).normalized() * bx + Vector3d(bx,0,0);
  quantityNames = {"Helmholtz-free-energy",
                  "Internal-energy","Entropy",
                  "Radius-of-gyration"};
}

protected:
  void growAt(const int & j, const double & phi){
    Vector3d r3 = xyz.at(j - 1),
              r2 = xyz.at(j - 2),
              r1 = xyz.at(j - 3);
    Vector3d i = (r3 - r2).normalized(), r21 = r1 - r2;
    Vector3d e = (r21 - (r21.dot(i) * i)).normalized();
    Vector3d k = i.cross(e);

    xyz.at(j) = r3 + b * (sin(theta)*cos(phi) * e -
                    sin(theta)*sin(phi) * k - cos(theta) * i);
    #ifdef DEBUG
    std::cout << "xyz " <<xyz.at(j) <<"end"<< std::endl;
    #endif
  }

public:
  std::list<double> getResults() const{
    double q = getPartition();
    double U = getInternal();
    double helmF = - kbt * log(q);
    double entropy = (U - helmF) / kbt;

    #ifdef DEBUG
    std::cout << q << " " << U <<" qu"<< std::endl;
    #endif

    std::list<double> ans = {helmF, U, entropy, getGyration()};
    return ans;
  }

protected:
  double getPartition() const{
    return pow(2 * pi,chainL - 3) *
          sum1 / sum2;
  }
  double getInternal() const{
    return sum3 / sum1;
  }
  double getGyration() const{
    return sumGyration/getSimulationTimes();
  }

  void updateSum(){
    double ev = totalEnergyv();
    double et = totalEnergyt();
    sum1 += bolzmanCof(ev);
    sum2 += 1 / bolzmanCof(et);
    sum3 += (ev + et)*bolzmanCof(ev);
    sumGyration += gyration();
    #ifdef DEBUG
    std::cout << ev << ' ' << et << std::endl;
    std::cout << sum1 << ' '<< sum2 << ' ' << sum3 << "sum" << std::endl;
    #endif

  }
private:
  void resetOthers(){
    sum1 = 0;sum2=0;sum3=0;
    sumGyration=0;
  }
};


void monteCarlo(const int &L, const int& M,
                Simulation & simulation,
                std::ostream& output = std::cout){
  std::list<std::list<double>> res;
  for(int i = 0; i != M; i++){
    res.push_back(simulation.runSimulation(L));
  }

  output<<"# number-of-conformers-to-sample(L)  number-of-repetitions(M)"
        <<std::endl;
  output << L << ' ' << M << std::endl;

  output<<"# quantity-name  ensamble-average  error-range" << std::endl;

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
  output << std::endl;
}



#endif

#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <const.h>
#include <functional>
#include <math.h>

//make random angle function getRandomPhi
std::random_device rdev{};
std::default_random_engine generator{rdev()};
// std::default_random_engine generator2{rdev()};
std::default_random_engine generator2(1);
// std::default_random_engine generator{};
std::uniform_real_distribution<double> angle(0, 2*pi);
std::uniform_real_distribution<double> unit(0,1);
//0,2pi均匀抽样
auto getUniformRandomPhi = bind(angle, generator);
auto unitRandom = bind(unit , generator2);

//t5 玻尔兹曼抽样 舍选法
double getBoltzmanPhi(const double &kbt, const double & kphi){
  auto prob = [&](const double & phi){
                return (double)exp(- 0.5 * kphi * (1 + cos(3 * phi))/kbt);
              };
  while(1){
    double phi = getUniformRandomPhi();
    if(unitRandom() <  prob(phi)){
      return phi;
    }
  }
}


#endif

#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <const.h>
#include <functional>

//make random angle function getRandomPhi
std::random_device rdev{};
std::default_random_engine generator{rdev()};
// std::default_random_engine generator{};
std::uniform_real_distribution<double> angle(0, 2*pi);
auto getUniformRandomPhi = bind(angle, generator);

#endif

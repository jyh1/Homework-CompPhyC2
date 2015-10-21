#ifndef STATISTICS_H

#include <list>
#include <cmath>
#include <functional>

template<typename T>
double totalBy(const std::list<T> & xs,
              std::function<double(T)> f = [](const T& x){return x;}){
  double tot = 0;
  for(auto i:xs){
    tot += f(i);
  }
  return tot;
}


template<typename T>
double meanBy(const std::list<T> & xs,
        std::function<double(T)> f = [](const T& x){return x;}){
  double tot = totalBy(xs, f);
  return (tot / xs.size());
}


template<typename T>
double variance(const std::list<T> & xs){
  double m = meanBy(xs);

  double tot = totalBy<double>(xs,[=](const T &x)
                        {return pow((x-m),2);});
  return tot/(xs.size() - 1);
}

template<typename T>
double standardDeviation(const std::list<T> & xs){
  return sqrt(variance(xs));
}

#define STATISTICS_H
#endif

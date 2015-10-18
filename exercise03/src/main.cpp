#include <iostream>
#include <fstream>
#include <random>
#include <list>
#include <vector>


#include <zmatrix.h>
#include <parsezmatrix.h>
#include <parse.h>
#include <const.h>


int main(int argc, char const *argv[]) {
  std::fstream ifs;

  if(argc == 1){
    ifs.open("../../data/randomcoil_in.dat", std::ifstream::in);
  }else{
    ifs.open(argv[1], std::ifstream::in);
  }


  std:ofstream rc_zmatrix("../results/rc_zmatrix.dat", std::ofstream::out);
  std::ofstream rc_xyz("../results/rc_xyz.dat", std::ofstream::out);

  double l, theta;
  int chainLength;

  //make parser
  list <Parser> requirement = {deleteComments,
                              makeDoubleParser(l),
                              makeAngleParser(theta),
                              makeIntParser(chainLength)};
  //run the parser
  parseSerial(requirement)(ifs);


  //make random angle function getRandomPhi
  std::random_device rdev{};
  std::default_random_engine generator{rdev()};
  // std::default_random_engine generator{};
  std::uniform_real_distribution<double> angle(0, 2*pi);
  auto getRandomPhi = bind(angle, generator);



  vector<Zmatrix*> res;
  res.reserve(chainLength + 1);

  res.push_back(new Zmatrix());//left empty
  res.push_back(new Zmatrix(1, 0,0,0));
  res.push_back(new Zmatrix(2, 1, l));
  res.push_back(new Zmatrix(3, 2, l, 1, theta));
  for(int i = 4; i <= chainLength; i++){
    res.push_back(new Zmatrix(i, i - 1, l, i - 2, theta, i - 3, getRandomPhi()));
  }

  for(auto i = res.begin() + 1 ; i != res.end(); i++){
    (*i) -> print(rc_zmatrix);
  }


  //convert to cartesian
  for(auto i = res.begin() + 1; i != res.end(); i++){
    (*i) -> toCartesian(res);
    (*i) -> print(rc_xyz);
  }

  return 0;
}

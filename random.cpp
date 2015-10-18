#include <random>
#include <iostream>
#include <functional>
using namespace std;

int main(int argc, char const *argv[]) {
  random_device rdev{};
  default_random_engine generator{rdev()};
  uniform_int_distribution<int> e(1,6);
  auto dice = bind(e, generator);
  cout << dice() << endl;
  cout << dice() << endl;
  return 0;
}

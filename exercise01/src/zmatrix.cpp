// #include <string>
// using std::string;
// #include "iostream"
// using namespace std;


class Zmatrix
{
public:
  int id, r3id, r2id,r1id;
  double l, theta, phi;
  Zmatrix(const int & idx, const int & r3idx, const double & lx, const int & r2idx, const double & thetax, const int & r1idx, const double & phix):
    id(idx), r3id(r3idx), r2id(r2idx), r1id(r1idx), l(lx), theta(thetax), phi(phix) { }
  // Zmatrix(){};
};

// int main()
// {
//   Zmatrix s(1,2,3,4,5,6,7);
//   cout << s.id << endl;
//   return 0;
// }

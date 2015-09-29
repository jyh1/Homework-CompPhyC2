// #include <string>
// using std::string;
// #include "iostream"
// using namespace std;


class Zmatrix
{
public:
  int id, r3id, r2id,r1id;
  double l, theta, phi;
  Zmatrix(int idx, int r3idx, double lx, int r2idx, double thetax, int r1idx, double phix):
    id(idx), r3id(r3idx), r2id(r2idx), r1id(r1idx), l(lx), theta(thetax), phi(phix) { }
  // Zmatrix(){};
};

// int main()
// {
//   Zmatrix s(1,2,3,4,5,6,7);
//   cout << s.id << endl;
//   return 0;
// }

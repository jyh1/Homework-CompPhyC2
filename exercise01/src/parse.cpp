#include <iostream>
#include <Eigen/Dense>
#include "zmatrix.h"
#include <string>

using namespace std;
using namespace Eigen;
using std::string;

const unsigned long int INF =  numeric_limits<streamsize>::max();
const int MAXNAMESIZE = 10;
const double degree = 0.017453292519943295;


int deleteComments(istream &input){
  while (input.peek() == '#'){
    if (input.peek() == EOF) {
      return -1;
    }
    input.ignore(INF, '\n');
  }
  return 0;
}


int deleteChars(istream &input, char c = ' '){
  while (input.peek() == c){
    input.ignore();
  }
  return 0;
}

string parseName(istream &input){
  deleteChars(input);
  char c[MAXNAMESIZE];
  input.get(c, MAXNAMESIZE, ' ');
  string name(c);
  return name;
}


Vector3d parseVector(istream &input){
  deleteComments(input);
  int id;
  input >> id;

  parseName(input);//discarded
  double d1, d2, d3;
  input >> d1 >> d2 >> d3;
  Vector3d v(d1, d2, d3);
  return v;
}


Zmatrix parseZmatrix(istream &input){
  int r1id, r2id, r3id, id;
  double l, theta, phi;
  deleteComments(input);
  input >> id;
  // cout << id << endl;
  parseName(input);//discarded
  input >> r3id >> l >> r2id >> theta >> r1id >> phi;
  theta *= degree; phi *= degree;
  Zmatrix data(id, r3id, l, r2id, theta, r1id, phi);
  // cout << data.r1id << endl;
  return data;
}


int main(int argc, char const *argv[]) {
  // Vector3d v = parseVector(cin);
  // cout << v << endl;
  string name;
  Zmatrix test = parseZmatrix(cin);
  cout << test.id << endl;
  return 0;
}

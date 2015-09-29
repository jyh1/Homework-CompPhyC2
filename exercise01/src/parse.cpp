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

enum ParseState {ParseSucced, ParseFailed};


typedef function<ParseState(istream &)> Parser;

Parser backward(Parser &p){  //go back if parser p failed
  return [p] (istream &input){
                auto pos = input.tellg();
                if (p(input) == ParseFailed){
                  input.clear();
                  input.seekg(pos);
                  return ParseFailed;
                }
                return ParseSucced;
              };
}


Parser makeAngleParser(double &phi){
  Parser p = [&phi](istream &input){
              double temp;
              if (input >> temp){
                phi = temp * degree;
                return ParseSucced;
              }
              return ParseFailed;
            };
  return (backward(p));
}

Parser makeIDParser(int &id){
  Parser p = [&id](istream &input){
              int temp;
              if (input >> temp){
                id = temp;
                return ParseSucced;
              }
              return ParseFailed;
        };
  return (backward(p));
}

Parser makeCharParser(char c){
  return [c](istream &input){
          if (input.peek() == c){
            input.get();
            return ParseSucced;
          }
          return ParseFailed;
        };
}

Parser parseUntilFailed(Parser &p){
  return [p](istream &input){
          while (p(input) == ParseSucced){};
          return ParseSucced;
        };
}





//Parser
Parser spaceParser = makeCharParser(' ');

ParseState commentParser(istream &input){
  if (input.peek() == '#'){
    input.ignore(INF, '\n');
    return ParseSucced;
  }
  return ParseFailed;
}




//Parser Combinator



// int deleteComments(istream &input){
//   while (input.peek() == '#'){
//     if (input.peek() == EOF) {
//       return -1;
//     }
//     input.ignore(INF, '\n');
//   }
//   return 0;
// }


// int deleteChars(istream &input, char c = ' '){
//   while (input.peek() == c){
//     input.ignore();
//   }
//   return 0;
// }
//
// string parseName(istream &input){
//   deleteChars(input);
//   char c[MAXNAMESIZE];
//   input.get(c, MAXNAMESIZE, ' ');
//   string name(c);
//   return name;
// }
//
//
//
//
// ParseState parseZmatrixcartesian(istream &input, Zmatrix &parsed){
//   double d1, d2, d3;
//   if (! (input >> d1 >> d2 >> d3)) {
//     return ParseFailed;
//   }
//   Vector3d temp(d1, d2, d3);
//   parsed.coord = temp;
//   parsed.type = cartesian;
//   return ParseSucced;
// }
//
//
// ParseState parseZmatrixmatrix(istream &input, Zmatrix &parsed){
//   if (! (input >> parsed.r3id >> parsed.l >> parsed.r2id >> parsed.theta
//               >> parsed.r1id >> parsed.phi)){
//     return ParseFailed;
//   }
//   parsed.theta *= degree; parsed.phi *= degree;
//   parsed.type = matrix;
//   return ParseSucced;
// }


int main(int argc, char const *argv[]) {
  double phi,d;
  char c;
  string s("2 3, 4 s");
  stringstream test(s);
  Parser p = makeAngleParser(phi);
  parseUntilFailed(p)(test);
  cout << phi << endl;
  test >> c;
  cout << c << endl;
  return 0;
}

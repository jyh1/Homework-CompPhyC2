#include <iostream>
#include <Eigen/Dense>
#include "zmatrix.h"
#include <string>
#include <list>
#include "parse.h"

using namespace std;
using namespace Eigen;
using std::string;


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

Parser makeStringParser(string &s){
  return [&s](istream &input){
            char c;
            if (! (input >> c)) return ParseFailed;
            s += c;
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

Parser makeNameParser(string &s){
  Parser p = makeStringParser(s);
  return parseUntilSucced(p, spaceParser);
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
          if (p(input) == ParseFailed) return ParseFailed;
          while (p(input) == ParseSucced){};
          return ParseSucced;
        };
}

Parser parseUntilSucced(Parser &p1, Parser &p2){
  return [p1, p2](istream &input){
          if (p2(input) == ParseSucced) return ParseFailed;
          while (p2(input) == ParseFailed) p1(input);
          return ParseSucced;
        };
}

Parser parseParallel(list<Parser> &parsers){  //return fail if none of the parser in list failed
  return [parsers](istream &input){
          for (auto i = parsers.begin(); i != parsers.end(); i++){
            if ( (*i)(input) == ParseSucced){
              return ParseSucced;
            }
          }
          return ParseFailed;
        };
}





//Parser
Parser spaceParser = makeCharParser(' ');

Parser deleteSpaces = parseUntilFailed(spaceParser);

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
  double phi=-1,d;
  int id=-1;
  char c, b[100];
  string ss("");
  list<Parser> parsers = {makeNameParser(ss),makeIDParser(id), makeAngleParser(phi)};
  cin.getline(b, 100);
  string s(b);
  stringstream test(s);
  cout << parseParallel(parsers)(test) <<endl;
  cout << phi << ' ' << id << ' '<< ss << endl;
  return 0;
}

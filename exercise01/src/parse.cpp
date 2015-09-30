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
                return ParseSucceed;
              };
}

Parser makeStringParser(string &s){
  return [&s](istream &input){
            char c;
            if (! (input >> c)) return ParseFailed;
            s += c;
            return ParseSucceed;
          };
}

Parser makeDoubleParser(double &phi){
  Parser p = [&phi](istream &input){
              double temp;
              if (input >> temp){
                phi = temp;
                return ParseSucceed;
              }
              return ParseFailed;
            };
  return (backward(p));
}

Parser makeIntParser(int &id){
  Parser p = [&id](istream &input){
              int temp;
              if (input >> temp){
                id = temp;
                return ParseSucceed;
              }
              return ParseFailed;
        };
  return (backward(p));
}

Parser makeNameParser(string &s){//name is the longest suffix of string before space
  Parser p = makeStringParser(s);
  return parseUntilSucceed(p, spaceParser);
}


// Parser makeCharParser(char c){
//   return [c](istream &input){
//           if (input.peek() == c){
//             input.get();
//             return ParseSucceed;
//           }
//           return ParseFailed;
//         };
// }

Parser makeCharsParser(string s){ //parse succeed if any char in the string s
  return [s](istream &input){
          if (s.find(input.peek()) != std::string::npos){
            input.get();
            return ParseSucceed;
          }
          return ParseFailed;
        };
}


Parser parseUntilFailed(Parser p){
  return [p](istream &input){
          if (p(input) == ParseFailed) return ParseFailed;
          while (p(input) == ParseSucceed){};
          return ParseSucceed;
        };
}

Parser parseUntilSucceed(Parser p1, Parser p2){
  return [p1, p2](istream &input){
          if (p2(input) == ParseSucceed) return ParseFailed;
          while (p2(input) == ParseFailed) p1(input);
          return ParseSucceed;
        };
}

Parser parseParallel(list<Parser> parsers){  //return fail if none of the parser in list failed
  return [parsers](istream &input){
          for (auto i = parsers.begin(); i != parsers.end(); i++){
            if ( (*i)(input) == ParseSucceed){
              return ParseSucceed;
            }
          }
          return ParseFailed;
        };
}

Parser parseSerial(list<Parser> parsers,Parser middle = deleteSpaces){
  Parser p = [parsers, middle](istream &input){
              for (auto i = parsers.begin(); i!= parsers.end(); i++){
                  middle(input);
                  if ( (*i)(input) == ParseFailed ){
                    return ParseFailed;
                  }
                }
              return ParseSucceed;
            };
  return (backward(p));
}


Parser alwaysSucceed(Parser p){
  return [p](istream &input){
          p(input);
          return ParseSucceed;
        };
}



//Parser
ParseState nullParser(istream &input){
  return ParseSucceed;
}

Parser spaceParser = makeCharsParser(" ");

Parser deleteSpaces = parseUntilFailed(spaceParser);

ParseState deleteComment(istream &input){
  if (input.peek() == '#'){
    input.ignore(INF, '\n');
    return ParseSucceed;
  }
  return ParseFailed;
}

Parser deleteComments = parseUntilFailed(deleteComment);



//Impletation of particular parser
Parser makeAngleParser(double &phi){
  return [&phi](istream& input){
            if (makeDoubleParser(phi)(input) == ParseSucceed){
              phi *= degree;
              return ParseSucceed;
            }
            return ParseFailed;
          };
}



Parser parseZmatrixcartesian(Zmatrix * & data){
  return [&data](istream &input){
          int id;
          string name("");
          double x, y, z;
          list<Parser> parsers = {makeIntParser(id), makeNameParser(name),
                                  makeDoubleParser(x), makeDoubleParser(y),
                                  makeDoubleParser(z)};
          if ( parseSerial(parsers)(input) == ParseSucceed ){
            data =  (new Zmatrix(id, x, y, z));
            return ParseSucceed;
          }
          return ParseFailed;
        };
}


Parser parseZmatrixsecond(Zmatrix *& data){
  return [&data](istream & input){
          int id,r3id;
          string name("");
          double l;
          list<Parser> parsers = {makeIntParser(id), makeNameParser(name),
                                  makeIntParser(r3id), makeDoubleParser(l)};
          if (parseSerial(parsers)(input) == ParseSucceed){
            data = (new Zmatrix(id, r3id, l));
            return ParseSucceed;
          }
          return ParseFailed;
        };
}

Parser parseZmatrixthird(Zmatrix *& data){
  return [&data](istream & input){
          int id,r3id, r2id;
          string name("");
          double l, theta;
          list<Parser> parsers = {makeIntParser(id), makeNameParser(name),
                                  makeIntParser(r3id), makeDoubleParser(l),
                                  makeIntParser(r2id), makeAngleParser(theta)};
          if (parseSerial(parsers)(input) == ParseSucceed){
            data = (new Zmatrix(id, r3id, l, r2id, theta));
            return ParseSucceed;
          }
          return ParseFailed;
        };
}

Parser parseZmatrixmatrix(Zmatrix *& data){
  return [&data](istream & input){
          int id,r3id, r2id, r1id;
          string name("");
          double l, theta, phi;
          list<Parser> parsers = {makeIntParser(id), makeNameParser(name),
                                  makeIntParser(r3id), makeDoubleParser(l),
                                  makeIntParser(r2id), makeAngleParser(theta),
                                  makeIntParser(r1id), makeAngleParser(phi)};
          if (parseSerial(parsers)(input) == ParseSucceed){
            data = (new Zmatrix(id, r3id, l, r2id, theta, r1id, phi));
            return ParseSucceed;
          }
          return ParseFailed;
        };
}


int main(int argc, char const *argv[]) {
  double phi=-1,d;
  int id=-1;
  char c, b[100],q;
  string ss("");
  list<Parser> parsers = {makeIntParser(id), makeDoubleParser(phi),makeNameParser(ss)};
  cin.getline(b, 100);
  string s(b);
  stringstream test(s), test2(s);
  // cout << parseSerial(parsers, nullParser)(test) <<endl;
  // cout << phi << ' ' << id << ' '<< ss << endl;
  // test >> q;
  // cout << q << endl;
  Zmatrix *matrix;
  cout << parseZmatrixmatrix(matrix)(test) << endl;
  (*matrix).print(cout);
  // cout << makeNameParser(ss)(test2) << endl;
  // test2 >> q;
  // cout << q << endl;
  return 0;
}

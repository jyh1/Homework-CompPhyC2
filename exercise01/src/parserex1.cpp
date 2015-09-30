#include <iostream>
#include <string>
#include <list>
#include "zmatrix.h"
#include "parse.h"
#include "parserex1.h"

using namespace std;
using std::string;

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

Parser parseZmatrix(Zmatrix *& data){
  return [&data] (istream & input){
      list<Parser> parsers = {parseZmatrixmatrix(data),
                              parseZmatrixthird(data),
                              parseZmatrixcartesian(data),
                              parseZmatrixsecond(data)};
      return parseParallel(parsers)(input);
    };
}

// //test part
// int main(int argc, char const *argv[]) {
//   double phi=-1,d;
//   int id=-1;
//   char c, b[100],q;
//   stringstream test1(" 1  A1  0.0  0.0  0.0");
//   stringstream test2(" 2  A2  1  1.5");
//   stringstream test3(" 3 A3  2  1.5  1  109.5");
//   stringstream test4(" 8 A8 7 1.5 6  109.5  5 180.0");
//   // list<stringstream> test = {test1,test2,test3,test4};
//   // cin >> id;
//   // cout << char(cin.peek()) << endl;
//   // cout << makeIntParser(id)(cin) << endl;
//   Zmatrix *aa;
//   // cout << parseZmatrixmatrix(aa)(test1) << endl;
//   Zmatrix *matrix;
//   // for (auto i = test.begin(); i != test.end(); i++){
//     parseZmatrix(matrix)(test1);
//     cout << (matrix -> type) << endl;
//     matrix->print(cout);
//     parseZmatrix(matrix)(test2);
//     cout << (matrix -> type) << endl;
//     matrix->print(cout);
//     parseZmatrix(matrix)(test3);
//     cout << (matrix -> type) << endl;
//     matrix->print(cout);
//     parseZmatrix(matrix)(test4);
//     cout << (matrix -> type) << endl;
//     matrix->print(cout);
//   //   (*matrix).print(cout);
//   // }
//   // cout << makeNameParser(ss)(test2) << endl;
//   // test2 >> q;
//   // cout << q << endl;
//   return 0;
// }

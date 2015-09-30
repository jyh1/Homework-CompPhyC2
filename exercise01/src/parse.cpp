#include <iostream>
#include <string>
#include <list>
#include "parse.h"

using namespace std;
using std::string;


Parser backward(Parser p){  //go back if parser p failed
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

Parser alwaysBackward(Parser p){  //go back if parser p failed
  return [p] (istream &input){
                auto pos = input.tellg();
                ParseState state = p(input);
                input.clear();
                input.seekg(pos);
                return state;
              };
}



//

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
              if (input >> temp)
                if (testBlankChar(input) == ParseSucceed){
                  id = temp;
                  return ParseSucceed;
                }
              return ParseFailed;
        };
  return (backward(p));
}

Parser makeNameParser(string &s){//name is the longest suffix of string before space
  Parser p = makeStringParser(s);
  return parseUntilSucceed(p, blankChar);
}



Parser makeCharsParser(string s){ //parse succeed if any char in the string s
  return [s](istream &input){
          if (s.find(input.peek()) != std::string::npos){
            input.get();
            return ParseSucceed;
          }
          return ParseFailed;
        };
}




//

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

Parser parseSerial(list<Parser> parsers,Parser middle){
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

Parser blankChar = makeCharsParser(" \t\n");

Parser testBlankChar = alwaysBackward(blankChar);

Parser deleteSpaces = parseUntilFailed(spaceParser);

#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <Eigen/Dense>
#include "zmatrix.h"
#include <string>
#include <list>

using namespace std;
using namespace Eigen;
using std::string;


const unsigned long int INF =  numeric_limits<streamsize>::max();
const int MAXNAMESIZE = 10;
const double degree = 0.017453292519943295;

enum ParseState {ParseSucceed, ParseFailed};

typedef function<ParseState(istream &)> Parser;



ParseState nullParser(istream &input);
extern Parser spaceParser;
extern Parser deleteSpaces;
extern Parser blankChar;
extern Parser testBlankChar;


Parser makeDoubleParser(double&);
Parser makeIntParser(int&);
Parser makeCharsParser(string&);
Parser makeStringParser(string&);
Parser makeNameParser(string &s);


Parser parseUntilFailed(Parser);
Parser parseUntilSucceed(Parser, Parser);
Parser parseParallel(list<Parser>);
Parser backward(Parser);
Parser alwaysBackward(Parser);


//important parser combinator
Parser parseParallel(list<Parser>);
Parser parseSerial(list<Parser> parsers,Parser middle);


#endif

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



extern Parser spaceParser;
extern Parser deleteSpaces;
extern Parser blankChar;
extern Parser testBlankChar;

Parser parseString(string &s);
Parser parseUntilSucceed(Parser, Parser);
Parser backward(Parser &p);

#endif

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

enum ParseState {ParseSucced, ParseFailed};

typedef function<ParseState(istream &)> Parser;

extern Parser spaceParser;
extern Parser deleteSpaces;

Parser parseString(string &s);

Parser parseUntilSucced(Parser &p1, Parser &p2);

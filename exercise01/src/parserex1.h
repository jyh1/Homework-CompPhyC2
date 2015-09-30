#ifndef PARSEREX1_H
#define PARSEREX1_H
#include <iostream>
#include <string>
#include <list>
#include "zmatrix.h"
#include "parse.h"

using namespace std;
using std::string;


const int MAXNAMESIZE = 10;
const double degree = 0.017453292519943295;

extern Parser deleteComments;
ParseState deleteComment(istream &input);


Parser makeAngleParser(double &phi);


Parser parseZmatrixcartesian(Zmatrix * & data);
Parser parseZmatrixsecond(Zmatrix *& data);
Parser parseZmatrixthird(Zmatrix *& data);
Parser parseZmatrixmatrix(Zmatrix *& data);


Parser parseZmatrix(Zmatrix *& data);

#endif

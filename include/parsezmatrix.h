#ifndef PARSEREX1_H
#define PARSEREX1_H
#include <list>
#include "zmatrix.h"
#include "parse.h"

using namespace std;


const double degree = 0.017453292519943295;

extern Parser deleteComments;
ParseState deleteComment(istream &input);


Parser makeAngleParser(double &phi);


Parser makeZmatrixcartesianParser(Zmatrix * &);
Parser makeZmatrixsecondParser(Zmatrix *&);
Parser makeZmatrixthirdParser(Zmatrix *&);
Parser makeZmatrixmatrixParser(Zmatrix *&);


Parser makeZmatrixParser (Zmatrix *&);
Parser makeZmatrixListParser(list<Zmatrix*> &);
Parser makeZmatrixFileParser(list<Zmatrix*> &);

//for t4 t5
Parser makeParametersParser(double & l, double & theta,
                            double &kbt, double & kphi);

Parser makeRequirementsParser(int & L, int &M);


#endif

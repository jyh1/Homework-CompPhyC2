#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <string>
#include <list>
#include <functional>
#include <limits>

using namespace std;
using std::string;


const unsigned long int INF =  numeric_limits<streamsize>::max();

const unsigned long int MAXLINE = 100;

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
Parser alwaysSucceed(Parser);
Parser parseWholeLine(Parser);


//important parser combinator
Parser parseParallel(list<Parser>);
Parser parseSerial(list<Parser> parsers,Parser middle = deleteSpaces);


#endif

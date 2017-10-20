/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: Lexer header class with private nextChar and
  public nextToken functions
*/

#ifndef Lexer_H
#define Lexer_H

class Token;
#include <iostream>
using namespace std;

class Lexer {
public:
  //Constructor
  Lexer(istream& refvar);

  //nextToken member function
  Token* nextToken();
  
private:
  istream& input;
  //int ttype;
  //string lexeme;
  char a;
  int line;
  int pos;

  //nextChar member function
  char nextChar();
};

#endif

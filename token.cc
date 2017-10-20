/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: Token body class with Token constructor and
  getters for Token.h's private fields
*/
  
#include "token.h"

//Constructor
Token::Token(int type, string tlexeme, int tline, int tpos){
  ttype = type;
  lexeme = tlexeme;
  line = tline;
  pos = tpos;
}

//Empty Destructor
Token::~Token() {

}

//Getters
int Token::getTtype() {
  return ttype;
}

string Token::getLexeme() {
  return lexeme;
}

int Token::getLine() {
  return line;
}

int Token::getPos() {
  return pos;
}



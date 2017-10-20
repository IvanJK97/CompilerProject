/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project : Lexer body class with constructor, nextChar, and
  nextToken function
*/

//V2
#include <cstdio>
#include <iostream>
#include <cctype>
#include "lexer.h"
#include "token.h"
using namespace std; 

//Constructor
Lexer::Lexer(istream& refvar): input(refvar) {
  line = 1;
  pos = 0;
  a = ' ';
}

//not sure whether should be input or refvar as param
char Lexer::nextChar(){ 

  //gets next character from istream
  char c = input.get();
  
  //end of file
  if (c == EOF){
    return '$';
  }
  //new line
  else if (c == '\n'){
    line++;
    pos = 0;
    return ' ';
  }  
  //comment  
  else if (c == '#'){
    while(c != '\n'){
      c = input.get();
      pos++;
    }
    line++;
    pos = 0;
    return ' ';
  }
  else{
    pos++;
    return c;
  }

  //Only use -> if its a pointer
}

Token* Lexer::nextToken(){
  string lexeme = "";
  //int i = a;
  // while (isspace(i) != 0){
  while (a == ' ' || a == '\t'){
    a = nextChar();
    //i = a;
  }
  int lineNumber = line;
  int posNumber = pos;
  if (a == '$'){
    Token* token1 = new Token(29, "EOF", lineNumber, posNumber);
    //New makes it a pointer already
    return token1;
  }
  else{
    if (isdigit(a) != 0){ //Not 0 means it is a digit
      while(isdigit((int) a) != 0){
	lexeme += a;
	a = nextChar();
      }
      //      if (isalpha(a) != 0){ Not 0 means it is an alphabet & error case
      //lexeme += a;  
      //a = nextChar();
      //Token* token2 = new Token(30, lexeme, lineNumber, posNumber);
      //return token2;
      //}
      //else{ 
      Token* token3 = new Token(Token::INTLIT, lexeme, lineNumber, posNumber); 
	return token3;
    }
    else if (isalpha(a) != 0){ //Not 0 means it is a letter
      while(isalpha(a) != 0){ 
	lexeme += a;
	a = nextChar();
      }
      if (lexeme == "if"){ //Keywords
	//a = nextChar();
	Token* token4 = new Token(Token::IF, lexeme, lineNumber, posNumber);
	return token4;
      }
      else if (lexeme == "else"){
	//a = nextChar();
	Token* token5 = new Token(Token::ELSE, lexeme, lineNumber, posNumber);
	return token5;
      }
      else if (lexeme == "while"){
	//a = nextChar();
	Token* token6 = new Token(Token::WHILE, lexeme, lineNumber, posNumber);
	return token6;
      }
      else if (lexeme == "function"){
	//a = nextChar();
	Token* token7 = new Token(Token::FUNCTION, lexeme,
				  lineNumber, posNumber);
	return token7;
      }
      else if (lexeme == "var"){
	//a = nextChar();
	Token* token8 = new Token(Token::VAR, lexeme, lineNumber, posNumber);
	return token8;
      }
      else if (lexeme == "return"){
	//a = nextChar();
	Token* token9 = new Token(Token::RETURN, lexeme,
				  lineNumber, posNumber);
	return token9;
      }
      else{ 
      	if (isdigit((int) a) != 0){ //Read if there are digits
				        //following the identifier;
       	  while (isdigit((int) a) != 0){
            lexeme += a;
	    a = nextChar();
	  }
	  //IDENT with numbers after
	  Token* token10 = new Token(Token::IDENT, lexeme, lineNumber, posNumber);
	  return token10;
        }
	else{
	  //IDENT without numbers after
	Token* token11 = new Token(Token::IDENT, lexeme,
				   lineNumber, posNumber);
	return token11;
	}
      }
    }
    else if (a == '"'){ 
      //lexeme += a; Including opening " in the stringlit token
      a = nextChar();
      while (a != '"'){
	lexeme += a;
	a = nextChar();
      }
      //lexeme += a; Including closing " in the stringlit token
      a = nextChar();
      Token* token12 = new Token(Token::STRINGLIT, lexeme,
				 lineNumber, posNumber);
      return token12;
    }
    else if (a == '+'){
      a = nextChar();
      Token* token13 = new Token(Token::PLUS, "+",
				 lineNumber, posNumber);
      return token13;
    }
    else if (a == '-'){
      a = nextChar();
      Token* token14 = new Token(Token::MINUS, "-",
				 lineNumber, posNumber);
      return token14;
    }
    else if (a == '*'){
      a = nextChar();
      Token* token15 = new Token(Token::TIMES, "*",
				 lineNumber, posNumber);
      return token15;
    }
    else if (a == '/'){
      a = nextChar();
      Token* token16 = new Token(Token::DIVIDE, "/",
				 lineNumber, posNumber);
      return token16;
    }
    else if (a == '('){
      a = nextChar();
      Token* token17 = new Token(Token::LPAREN, "(",
				 lineNumber, posNumber);
      return token17;
    }
    else if (a == ')'){
      a = nextChar();
      Token* token18 = new Token(Token::RPAREN, ")",
				 lineNumber, posNumber);
      return token18;
    }
    else if (a == '{'){
      a = nextChar();
      Token* token19 = new Token(Token::LBRACE, "{",
				 lineNumber, posNumber);
      return token19;
    }
    else if (a == '}'){
      a = nextChar();
      Token* token20 = new Token(Token::RBRACE, "}",
				 lineNumber, posNumber);
      return token20;
    }
    else if (a == ','){
      a = nextChar();
      Token* token21 = new Token(Token::COMMA, ",",
				 lineNumber, posNumber);
      return token21;
    }
    else if (a == ';'){
      a = nextChar();
      Token* token22 = new Token(Token::SEMICOLON, ";",
				 lineNumber, posNumber);
      return token22;
    }
    else if (a == '='){
      a = nextChar();
      if (a == '='){ 
	a = nextChar();
	Token* token23 = new Token(Token::EQ, "==", lineNumber, posNumber);
	return token23;
      }
      else{ 
	Token* token24 = new Token(Token::ASSIGN, "=",
				   lineNumber, posNumber);
	return token24;
      }
    }
    else if (a == '!'){
      a = nextChar();
      if (a == '='){  
	a = nextChar();
	Token* token25 = new Token(Token::NE, "!=", lineNumber, posNumber);
	return token25;
      }
      else{ 
	Token* token26 = new Token(Token::ERROR, "error",
				   lineNumber, posNumber);
	return token26;
      }
    }
    else if (a == '<'){
      a = nextChar();
      if (a == '='){
	a = nextChar();
	Token* token27 = new Token(Token::LE, "<=", lineNumber, posNumber);
	return token27;
      }
      else{
	Token* token28 = new Token(Token::LT, "<", lineNumber, posNumber);
	return token28;
      }
    }
    else if(a == '>'){
      a = nextChar();
      if (a == '='){
	a = nextChar();
	Token* token29 = new Token(Token::GE, ">=", lineNumber, posNumber);
	return token29;
      }
      else{
	Token* token30 = new Token(Token::GT, ">", lineNumber, posNumber);
	return token30;
      }
    }
    else if(a == '&'){
      a = nextChar();
      if (a == '&'){
	a = nextChar();
	Token* token31 = new Token(Token::AND, "&&", lineNumber, posNumber);
	return token31;
      }
      else{
	Token* token32 = new Token(Token::ERROR, "error",
				   lineNumber, posNumber);
	return token32;
      }
    }
    else if(a == '|'){
      a = nextChar();
      if (a == '|'){
	a = nextChar();
	Token* token33 = new Token(Token::OR, "||", lineNumber, posNumber);
	return token33;
      }
      else{
	Token* token34 = new Token(Token::ERROR, "error",
				   lineNumber, posNumber);
	return token34;
      }
    }
    else{
      a = nextChar();
      Token* token35 = new Token(Token::ERROR, "error",
				 lineNumber, posNumber);
      return token35;
    }
  }
}

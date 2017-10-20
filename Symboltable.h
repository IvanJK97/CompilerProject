/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: Symboltable header class
*/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "templatestack.cc"
#include "hashTable.h"
#include <iostream>
#include <sstream>
using namespace std;

class Symboltable {
public:
  //Constructor
  Symboltable();

  //Member Functions
  void resetCount();
  void enterScope();
  HashTable exitScope();
  int addSymbol(string sym);
  int getUniqueSymbol(string sym);

private:
  //Use templatestack to make a stack of hashTables
  Stack<HashTable> s;
  int counter;
  
};
#endif

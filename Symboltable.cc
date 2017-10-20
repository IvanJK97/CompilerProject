/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: Symboltable to define scope for the variables
*/

#include "Symboltable.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

Symboltable::Symboltable(){
  s = Stack<HashTable>(100);
  //counter starts at 1 and increments everytime
  //addSymbol is called successfully
  counter = 1;
}

void Symboltable::resetCount(){
  //reset counter when entering new function
  counter = 1;
}

void Symboltable::enterScope(){
  s.push(HashTable(97));
}

HashTable Symboltable::exitScope(){
  return s.pop();
}

int Symboltable::addSymbol(string sym){
  HashTable h = s.peek();
  //converts string to const char*
  const char* cstr = sym.c_str();
  
  if(h.inTable(cstr)){
    //var name is repeated, so cannot be added again
    return 0;
  }
  else{
    h.add(cstr, counter);
    counter++;
    return 1;
  }
}

int Symboltable::getUniqueSymbol(string sym){
  int i = s.getTos();
  const char* cstr = sym.c_str();

  //start at top of stack of hashtables and
  //loop till stack is empty
  for(; i >= 0; i--){
    //check at each hashTable whether string is in hashTable
    if(s[i].inTable(cstr)){
      //cout << s.getIndex(i).getKey(cstr) << endl;
      return s[i][cstr];
    }
  }
  return -1;
}



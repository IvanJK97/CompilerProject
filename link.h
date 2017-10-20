/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: Link header class from Femister's demos used in
  hashTable.h
*/

#ifndef LINK__H
#define LINK_H
#include <iostream>
#include <fstream>
using namespace std;


class Link{
public:
  const char *key;  //used to identify the Links; should be unique
  int data;  //the actual information
  Link *next;

  /*Set key=k, data=d, next=nx*/
  Link(const char *k, int d = 0, Link *nx = NULL);

  /*Make a copy of the link t. Note that we copy the pointer, thus not
  a deep copy
  */
  Link(const Link & t);

  /*Print out in the form  (2, 2.7)  */
  friend ostream& operator <<(ostream& out, const Link& t);
};
#endif

/*
  CSE 109
  Ivan Kok
  ivk215
  Compiler Project: hashTable body class adapted from Femister's demos
*/

#include "hashTable.h"
#include <iostream>
using namespace std;

HashTable::HashTable(int n):size(n){
  check(n > 2 && prime(n),
	"(HashTable()) Need positive prime number for table size");
  table = new Link*[n];
  check(table != NULL,"(HashTable(int)) Heap overflow");
  for(int j = 0; j < size; j++)
    table[j] = NULL;
}

/*
HashTable::HashTable(const HashTable& t):size(t.size){
  table = new Link*[size];
  check(table != NULL, "(HashTable(HashTable)) Heap overflow");
  for(int j = 0; j < size; j++)
    table[j] = t.table[j];
}

HashTable::~HashTable(){
  for(int j = 0; j < size; j++)
    if(table[j] != NULL)
      delete table[j];
delete [] table;
}
*/

HashTable& HashTable::add(const char *str, int data){
  int h;
  check(!inTable(str),"(add()) Duplicate key");
  h = Hash(str).hash() % size;
  table[h] = new Link(str, data, table[h]);
  check(table[h] != NULL, "(add()) Heap overflow");
  return *this;
}

bool HashTable::inTable(const char *k) const{
  int h;
  Link *temp;
  h = Hash(k).hash() % size;
  temp = table[h];
  while(temp != NULL && strcmp(temp->key,k) != 0)
    temp = temp->next;
  return temp != NULL;
}

const int HashTable::getKey(const char *str) const{
  int h;
  Link *temp;  // double x = t["Lehigh"];
  check(inTable(str),"([]const) Missing key");
  h = Hash(str).hash() % size;
  temp = table[h];
  while(temp != NULL && strcmp(temp->key,str) != 0)
    temp = temp->next;
  return temp->data;
}

/*
int HashTable::operator [](const char *str){
  int h;
  Link *temp;
  if(!inTable(str)) // t["Lehigh"] = 5;
    *this += str;
  h = Hash(str).hash() % size;
  temp = table[h];
  while(temp != NULL && strcmp(temp->key, str) != 0)
    temp = temp->next;
  return temp->data;
}
*/

const int HashTable::operator [](const char *str) const{
  int h;
  Link *temp;  // double x = t["Lehigh"];
  check(inTable(str),"([]const) Missing key");
  h = Hash(str).hash() % size;
  temp = table[h];
  while(temp != NULL && strcmp(temp->key,str) != 0)
     temp = temp->next;
  return temp->data;
}

//HashTable& HashTable::operator +=(const char *k){
//return add(k, 0);
//}

void HashTable::remove(const char *str){
  int h;
  Link *tempa, *tempb;
  check(inTable(str),"(remove()) Missing key");
  h = Hash(str).hash() % size;
  if(strcmp(table[h]->key, str) == 0){
    tempa = table[h];
    table[h] = table[h]->next;
  }
  else{
    tempb = table[h];
    while(strcmp(tempb->next->key, str) != 0)
      tempb = tempb->next;
    tempa = tempb->next;
    tempb->next = tempb->next->next;
  }
  delete tempa;
}

ostream& operator <<(ostream& out, const HashTable& h){
  Link *temp;
  for(int j = 0; j < h.size; j++){
    temp = h.table[j];
    if(temp != NULL)
      out << "-----------[ " << j << "]------\n";
    while(temp != NULL){
      out << *temp << endl;
      temp = temp->next;
    }
  }
  return out;
}

bool HashTable::prime(int k){
  int divisor;
  if(k < 0)
    k = -k;
  divisor = 2;
  while(divisor * divisor <= k && k % divisor != 0)
    divisor++;
  return k % divisor != 0;
}
  
void HashTable::check(bool b, const char *mess){
  if(!b){
    cerr << "ERROR[HashTable]: " << mess << endl;
    exit(1);
  }
}

//
// Created by Admin on 27/12/2024.
//

#include "AVisitor.h"
#include "A.h"

#include <iostream>

using namespace My;
using namespace std;

int main() {
  AVisitor v;

  A a;
  B b;
  C c;
  A* ptrA[3] = { &a,&b,&c };

  v.Visit(ptrA[0]);
  v.Visit(ptrA[1]);
  v.Visit(ptrA[2]);
}
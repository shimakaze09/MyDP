//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Basic/vtable.h>

#include <iostream>

using namespace My;
using namespace std;

struct A {
  virtual ~A() = default;
};

struct B : A {};

struct C : A {};

int main() {
  cout << vtable_of<A>::get() << endl;
  cout << vtable_of<B>::get() << endl;
  cout << vtable_of<C>::get() << endl;
  A a;
  B b;
  cout << vtable(&a) << endl;
  cout << vtable_is<B>(&a) << endl;
  cout << vtable_is<A>(&a) << endl;
  cout << vtable_is<B>((A*)&b) << endl;
  cout << vtable_is<A>((A*)&b) << endl;
}

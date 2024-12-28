//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Reflection/Reflection.h>

#include <iostream>
#include <string>

using namespace std;
using namespace My;

class C {
 public:
  void Foo() { cout << n << " non-const Foo" << endl; }

  void Bar() const { cout << n << " const Bar" << endl; }

  int n{0};
};

int main() {
  Reflection<C>::Instance().Regist(&C::Foo, "Foo").Regist(&C::Bar, "Bar");

  C c{3};
  const C cc{4};

  Reflection<C>::Instance().Call("Foo", c);
  Reflection<C>::Instance().Call("Foo", &c);
  Reflection<C>::Instance().Call("Bar", cc);
}
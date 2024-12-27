//
// Created by Admin on 27/12/2024.
//

#include <MyDP/Visitor.h>

#include <iostream>
#include <memory>

using namespace My;
using namespace std;

struct A {
  virtual ~A() = default;
};

struct B : A {};

struct C : A {};

struct GetName : public Visitor<GetName, A, std::shared_ptr> {
  GetName() {
    Regist<A>();
    Regist<B>();
  }

 private:
  friend class VisitorType;

  void ImplVisit(shared_ptr<A>) {
    cout << "GetName::ImplVisit(shared_ptr<A>)" << endl;
  }

  void ImplVisit(shared_ptr<B>) {
    cout << "GetName::ImplVisit(shared_ptr<B>)" << endl;
  }

  string rst;
};

int main() {
  GetName v;
  v.Regist([](shared_ptr<C>) { cout << "Lambda(shared_ptr<C>)" << endl; });

  auto a = make_shared<A>();
  auto b = make_shared<B>();
  auto c = make_shared<C>();
  shared_ptr<A> ptrA[3] = {a, b, c};

  v.Visit(ptrA[0]);
  v.Visit(ptrA[1]);
  v.Visit(ptrA[2]);

  return 0;
}
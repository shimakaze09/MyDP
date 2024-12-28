//
// Created by Admin on 27/12/2024.
//

#include <MyDP/Visitor/Visitor.h>

#include <iostream>
#include <memory>

using namespace My;
using namespace std;

struct A {
  virtual ~A() = default;
};

struct B : A {};

struct C : A {};

int main() {
#ifdef NDEBUG
  cout << "You should run test_00_Visitor_03_robustness in Debug mode." << endl;
  return 1;
#endif  // !NDEBUG

  {
    cout << "[ test 0 ] hasn't register struct C" << endl;
    BasicRawPtrVisitor<A> v;
    v.Regist([](A*) { cout << "Lambda(A*)" << endl; });
    v.Regist([](B*) { cout << "Lambda(B*)" << endl; });
    // v.Regist([](C*) { cout << "Lambda(C*)" << endl; });
    A a;
    B b;
    C c;
    A* ptrA[3] = {&a, &b, &c};

    v.Visit(ptrA[0]);
    v.Visit(ptrA[1]);
    v.Visit(ptrA[2]);
    cout << endl;
  }

  {
    cout << "[ test 1 ] repeatedly regist struct C" << endl;
    BasicSharedPtrVisitor<A> v;
    v.Regist([](shared_ptr<A>) { cout << "Lambda(shared_ptr<A>)" << endl; });
    v.Regist([](shared_ptr<B>) { cout << "Lambda(shared_ptr<B>)" << endl; });
    v.Regist([](shared_ptr<C>) { cout << "Lambda(shared_ptr<C>)" << endl; });
    v.Regist([](shared_ptr<C>) { cout << "Lambda(shared_ptr<C>)" << endl; });

    auto a = make_shared<A>();
    auto b = make_shared<B>();
    auto c = make_shared<C>();
    shared_ptr<A> ptrA[3] = {a, b, c};

    v.Visit(ptrA[0]);
    v.Visit(ptrA[1]);
    v.Visit(ptrA[2]);
    cout << endl;
  }

  return 0;
}
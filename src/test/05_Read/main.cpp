//
// Created by Admin on 30/12/2024.
//

#include <MyDP/Basic/Read.h>

#include <MyDP/Reflection/Reflection.h>

#include <iostream>

#include <array>

using namespace My;
using namespace std;

struct A {
  Read<A, float> f{2.f};
};

struct B {
  const float f{1.f};
};

struct C {
  float* pf{nullptr};
};

struct D {
  Read<A, float*> pf;
};

int main() {
  A a;

  Reflection<A>::Instance().SetName("A").Regist(&A::f, "f");

  cout << Reflection<A>::Instance().Var<float>("f").Of(a) << endl;
  Reflection<A>::Instance().Var<float>("f").Of(a) = 3.f;
  cout << Reflection<A>::Instance().Var<float>("f").Of(a) << endl;

  static_assert(
      !std::is_const_v<
          std::remove_reference_t<decltype(*std::declval<const C>().pf)>>);
  static_assert(
      std::is_const_v<
          std::remove_reference_t<decltype(*std::declval<const D>().pf)>>);

  return 0;
}
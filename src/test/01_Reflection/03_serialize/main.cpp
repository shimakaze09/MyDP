//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Reflection/MemVarVisitor.h>
#include <MyDP/Reflection/Reflection.h>
#include <MyDP/Visitor/MultiVisitor.h>

#include "nameof.hpp"

#include <iostream>
#include <string>

using namespace std;
using namespace My;

struct Figure {
  virtual ~Figure() = default;
};

struct Sphere : Figure {
  int radius{0};
};

struct Square : Figure {
  float sideLength{0.f};
};

struct Cosmetics {
  virtual ~Cosmetics() = default;
};

struct Lipstick : Cosmetics {
  string name{"mac"};
};

struct Lipglaze : Cosmetics {
  float color{0.f};
};

template <typename Obj>
struct MemVarSerializer : MemVarVisitor<MemVarSerializer<Obj>, Obj> {
  MemVarSerializer() { this->Regist<float, int, string>(); }

  template <typename T>
  void ImplVisit(const T& var) {
    cout << var;
  }

  virtual void ImplVisit(const string& var) { cout << "\"" << var << "\""; }
};

struct Serializer : RawPtrMultiVisitor<Serializer, Figure, Cosmetics> {
  Serializer() { Regist<Sphere, Square, Lipstick, Lipglaze>(); }

  template <typename T>
  void ImplVisit(T* e) {
    static MemVarSerializer<T> ms;
    ms.SetObj(e);
    cout << "{" << endl;
    cout << "\"type\": \"" << Reflection<T>::Instance().GetName() << "\""
         << endl;
    for (auto nv : Reflection<T>::Instance().Vars()) {
      cout << "\"" << nv.first << "\"" << ": ";
      ms.Visit(nv.second);
      cout << endl;
    }
    cout << "}" << endl;
  }
};

int main() {
  Reflection<Sphere>::Instance()
      .SetName(nameof::nameof_type<Sphere>().data())
      .Regist(&Sphere::radius, NAMEOF(&Sphere::radius).data());

  Reflection<Square>::Instance()
      .SetName(nameof::nameof_type<Square>().data())
      .Regist(&Square::sideLength, NAMEOF(&Square::sideLength).data());

  Reflection<Lipstick>::Instance()
      .SetName(nameof::nameof_type<Lipstick>().data())
      .Regist(&Lipstick::name, NAMEOF(&Lipstick::name).data());

  Reflection<Lipglaze>::Instance()
      .SetName(nameof::nameof_type<Lipglaze>().data())
      .Regist(&Lipglaze::color, NAMEOF(&Lipglaze::color).data());

  Serializer serializer;
  Sphere a;
  Square b;
  Figure* figures[2] = {&a, &b};
  Lipstick c;
  Lipglaze d;
  Cosmetics* cosmetics[2] = {&c, &d};

  for (auto f : figures)
    serializer.Visit(f);
  for (auto c : cosmetics)
    serializer.Visit(c);
}
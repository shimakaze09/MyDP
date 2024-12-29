//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Reflection/MemVarVisitor.h>
#include <MyDP/Reflection/Reflection.h>
#include <MyDP/Visitor/MultiVisitor.h>

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
  Reflection<Sphere>::Instance().SetName("Sphere").Regist(&Sphere::radius,
                                                          "radius");

  Reflection<Square>::Instance().SetName("Square").Regist(&Square::sideLength,
                                                          "sideLength");

  Reflection<Lipstick>::Instance()
      .SetName("Lipstick")
      .Regist(&Lipstick::name, "name");

  Reflection<Lipglaze>::Instance()
      .SetName("Lipglaze")
      .Regist(&Lipglaze::color, "color");

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

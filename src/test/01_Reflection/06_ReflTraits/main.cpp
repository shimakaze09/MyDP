//
// Created by Admin on 30/12/2024.
//

#include <MyDP/Reflection/Reflection.h>
#include <MyDP/Reflection/VarPtrVisitor.h>

#include <array>
#include <iostream>

using namespace std;
using namespace My;

struct Figure {
  virtual ~Figure() = default;
};

struct Sphere : Figure {
  int radius{1};
};

struct Square : Figure {
  float sideLength{2.f};
};

struct Cosmetics {
  virtual ~Cosmetics() = default;
};

struct Lipstick : Cosmetics {
  string name{"mac"};
};

struct Lipglaze : Cosmetics {
  std::array<float, 3> color{0.9f, 0.1f, 0.1f};
};

class VarSerializer : public VarPtrVisitor<VarSerializer>,
                      public ReflTraitsVisitor {
 public:
  VarSerializer() {
    VarPtrVisitor<VarSerializer>::RegistC<float, string, array<float, 3>>();
  }

  using VarPtrVisitor<VarSerializer>::Regist;
  using ReflTraitsVisitor::Visit;

 protected:
  template <typename T>
  void ImplVisit(const T& p) {
    cout << p;
  }

  template <typename T, size_t N>
  void ImplVisit(const array<T, N>& p) {
    for (auto var : p)
      cout << var << ", ";
  }

  void ImplVisit(const string& p) { cout << "\"" << p << "\""; }

 private:
  virtual void Receive(const string& name,
                       const map<string, shared_ptr<VarPtrBase>>& nv) override {
    cout << "{" << endl;
    cout << "  \"type\": \"" << name << "\"" << endl;
    for (auto [n, v] : nv) {
      cout << "  \"" << n << "\"" << ": ";
      VarPtrVisitor<VarSerializer>::Visit(v);
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

  VarSerializer vs;
  ReflTraitsIniter::Instance().Init(vs);

  vs.Regist([](const int& v) { cout << v; });

  Sphere a;
  Square b;
  Figure* figures[2] = {&a, &b};
  Lipstick c;
  Lipglaze d;
  Cosmetics* cosmetics[2] = {&c, &d};

  for (auto f : figures)
    vs.Visit(f);
  for (auto c : cosmetics)
    vs.Visit(c);
}

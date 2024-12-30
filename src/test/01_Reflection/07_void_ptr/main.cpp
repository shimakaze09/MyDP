//
// Created by Admin on 30/12/2024.
//

#include <MyDP/Reflection/Reflection.h>

#include <iostream>
#include <string>

using namespace std;
using namespace My;

struct Point {
  virtual ~Point() = default;

  Point(float x = 1.f, float y = 1.f) : x{x}, y{y} {}

  float x;
  float y;
};

int main() {
  Reflection<Point>::Instance()
      .SetName("Point")
      .Regist(&Point::x, "x")
      .Regist(&Point::y, "y")
      .RegistConstructor([](float x, float y) { return new Point(x, y); });

  auto ptr = ReflectionMngr::Instance().Create("Point", 1.f, 2.f);
  auto refl = ReflectionMngr::Instance().GetReflction(ptr);

  for (auto [n, v] : refl->VarPtrs(ptr))
    cout << n << endl;
}
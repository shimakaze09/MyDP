//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Reflection.h>

#include <iostream>
#include <string>

using namespace std;
using namespace My;

struct Point {
  float x;
  float y;
};

int main() {
  Reflection<Point>::Regist(&Point::x, "x");
  Reflection<Point>::Regist(&Point::y, "y");
  Point p;
  Reflection<Point>::Var<float>("x").Of(p) = 3;
  Reflection<Point>::Var<float>("y").Of(p) = 4;
  for (auto nv : Reflection<Point>::Vars()) {
    cout << nv.first << ": ";
    cout << nv.second.As<float>().Of(p) << endl;
  }
}
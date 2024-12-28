//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Signal/Signal.h>

#include <iostream>

using namespace My;
using namespace std;

int main() {
  Signal<int, int> mouseMoved;
  auto connection =
      mouseMoved.Connect([](int x, int y) { cout << x << ", " << y << endl; });
  mouseMoved.Emit(3, 4);
}
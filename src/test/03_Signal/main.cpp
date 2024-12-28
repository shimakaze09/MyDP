//
// Created by Admin on 28/12/2024.
//

#include <MyDP/Signal/Signal.h>

#include <iostream>

using namespace My;
using namespace std;

int main() {
  Signal<float, int, int> s0;
  s0.Connect([](int, int n) {
      cout << n << endl;
      });
  s0.Emit(1.f, 2, 3);
}

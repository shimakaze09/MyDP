//
// Created by Admin on 29/12/2024.
//

#include <MyDP/Basic/Dirty.h>

#include <iostream>

using namespace My;
using namespace std;

int main() {
  Dirty<float> height([](float& h) { h = 3.f; });
  auto h0 = height.Get();
  auto h1 = height.Get();
  cout << h1 << endl;
}

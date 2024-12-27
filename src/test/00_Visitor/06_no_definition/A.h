//
// Created by Admin on 27/12/2024.
//

#pragma once

namespace My {
struct A {
  virtual ~A() = default;
};

struct B : A {};

struct C : A {};
}  // namespace My

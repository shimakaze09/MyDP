//
// Created by Admin on 27/12/2024.
//

#pragma once
#include <MyDP/Visitor.h>

namespace My {
struct A;
struct B;
struct C;

class AVisitor final : public RawPtrVisitor<AVisitor, A> {
 public:
  AVisitor();

 protected:
  void ImplVisit(A*);
  void ImplVisit(B*);
  void ImplVisit(C*);
};
}  // namespace My

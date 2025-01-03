//
// Created by Admin on 28/12/2024.
//

#pragma once

#include "../Visitor/Visitor.h"
#include "MemVar.h"

namespace My {
template <typename Impl, typename Obj>
struct MemVarVisitor
    : RawPtrVisitor<MemVarVisitor<Impl, Obj>, MemVarBase<Obj>> {
  template <typename... Ts>
  inline void Regist() noexcept {
    RawPtrVisitor<MemVarVisitor<Impl, Obj>,
                  MemVarBase<Obj>>::template Regist<MemVar<Ts Obj::*>...>();
  }

  template <typename... Funcs>
  inline void Regist(Funcs&&... func) noexcept {
    RawPtrVisitor<MemVarVisitor<Impl, Obj>, MemVarBase<Obj>>::template Regist<
        Funcs...>(std::forward<Funcs>(func)...);
  }

  template <typename T>
  void ImplVisit(MemVar<T Obj::*>* memvar) {
    static_cast<Impl*>(this)->ImplVisit(memvar->Of(obj));
  }

  void SetObj(Obj* obj) { this->obj = obj; }

 private:
  Obj* obj{nullptr};
};
}  // namespace My
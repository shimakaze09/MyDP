//
// Created by Admin on 30/12/2024.
//

#pragma once

#include "../Basic/vtable.h"
#include "ReflectionBase.h"

namespace My {
template <typename Obj>
struct Reflection;

struct ReflectionMngr {
  static inline ReflectionMngr& Instance();

  // Args must match exactly
  template <typename... Args>
  void* Create(const std::string& name, Args... args) const;

  inline ReflectionBase* GetReflction(const void* obj) const;

 private:
  template <typename Obj>
  friend struct Reflection;

  template <typename Obj>
  void RegistRefl(ReflectionBase* refl);

  template <typename Func>
  void RegistConstructor(const std::string& name, Func&& func);

 private:
  std::map<const void*, ReflectionBase*> vt2refl;
  std::map<std::string, std::function<void*(void*)>> constructors;

  ReflectionMngr() = default;
};
}  // namespace My

#include "detail/ReflectionMngr.inl"
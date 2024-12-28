//
// Created by Admin on 28/12/2024.
//

#pragma once

#include "MemFunc.h"
#include "MemVar.h"

#include <map>
#include <string>

namespace My::detail::Reflection_ {
template <typename Mem>
struct Regist;
template <typename Obj, typename RObj, typename Ret, typename... Args>
struct Call;
}  // namespace My::detail::Reflection_

namespace My {
template <typename Obj>
struct Reflection final {
  inline static Reflection& Instance() noexcept;

  template <typename T>
  inline Reflection& Regist(T Obj::* ptr, const std::string& name) noexcept;

  template <typename U = void>
  inline const MemVar<U Obj::*> Var(const std::string& name);

  inline const std::map<std::string, MemVar<void * Obj::*>> Vars()
      const noexcept;

  template <typename Ret = void, typename RObj, typename... Args>
  inline Ret Call(const std::string& name, RObj&& obj, Args&&... args);

 private:
  std::map<std::string, MemVar<void * Obj::*>> n2mv;
  std::map<std::string, MemFunc<void (Obj::*)(void*)>> n2mf;
  std::map<std::string, MemFunc<void (Obj::*)(void*) const>> n2mfc;
  Reflection() = default;

  template <typename Mem>
  friend struct detail::Reflection_::Regist;
  template <typename Obj, typename RObj, typename Ret, typename... Args>
  friend struct detail::Reflection_::Call;
};
}  // namespace My

#include "detail/Reflection.inl"
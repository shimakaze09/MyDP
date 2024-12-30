//
// Created by Admin on 28/12/2024.
//

#pragma once

#include "MemFunc.h"
#include "MemVar.h"
#include "ReflTraits.h"
#include "ReflectionMngr.h"

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
struct Reflection final : ReflectionBase {
  static Reflection& Instance() noexcept;

  Reflection& SetName(const std::string& name) noexcept;

  const std::string& GetName() noexcept { return name; }

  // call after SetName()
  Reflection& RegistConstructor();
  template <typename Func>
  Reflection& RegistConstructor(Func&& func);

  // member variable pointer, member function pointer
  template <typename T>
  Reflection& Regist(T Obj::* ptr, const std::string& name) noexcept;

  template <typename U>
  MemVar<U Obj::*> Var(const std::string& name) const noexcept;

  std::map<std::string, MemVarBase<Obj>*> Vars() const noexcept;

  std::map<std::string, std::shared_ptr<VarPtrBase>> VarPtrs(
      Obj& obj) const noexcept;
  std::map<std::string, std::shared_ptr<const VarPtrBase>> VarPtrs(
      const Obj& obj) const noexcept;

  template <typename Ret = void, typename RObj, typename... Args>
  Ret Call(const std::string& name, RObj&& obj, Args&&... args);

  std::map<std::string, MemFuncBase<Obj>*> Funcs() const noexcept {
    return n2mf;
  }

  std::map<std::string, MemCFuncBase<Obj>*> CFuncs() const noexcept {
    return n2mcf;
  }

 private:
  virtual std::map<std::string, std::shared_ptr<VarPtrBase>> VarPtrs(
      void* obj) const override;
  virtual std::map<std::string, std::shared_ptr<const VarPtrBase>> VarPtrs(
      const void* obj) const override;

 private:
  std::map<std::string, MemVarBase<Obj>*> n2mv;
  std::map<std::string, MemFuncBase<Obj>*> n2mf;
  std::map<std::string, MemCFuncBase<Obj>*> n2mcf;
  std::string name;

  Reflection() {
    ReflTraitsIniter::Instance().Regist<Obj>();
    ReflectionMngr::Instance().RegistRefl<Obj>(this);
  }

  template <typename Mem>
  friend struct detail::Reflection_::Regist;
  template <typename Obj, typename RObj, typename Ret, typename... Args>
  friend struct detail::Reflection_::Call;
};
}  // namespace My

#include "detail/Reflection.inl"
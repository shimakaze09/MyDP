//
// Created by Admin on 28/12/2024.
//

#pragma once

#include <cassert>

#ifndef NDEBUG
#include <iostream>
#endif  // !NDEBUG

#include "../../Basic/Read.h"

namespace My {
template <typename Obj>
Reflection<Obj>& Reflection<Obj>::Instance() noexcept {
  static Reflection instance;
  return instance;
}

template <typename Obj>
Reflection<Obj>& Reflection<Obj>::SetName(const std::string& name) noexcept {
  this->name = name;
  return *this;
}

template <typename Obj>
Reflection<Obj>& Reflection<Obj>::RegistConstructor() {
  if (!name.empty())
    ReflectionMngr::Instance().RegistConstructor(
        name, []() -> void* { return reinterpret_cast<void*>(new Obj); });
#ifndef NDEBUG
  else {
    std::cerr << "WARNING::Reflection::RegistConstructor:" << std::endl
              << "\t" << "name is empty" << std::endl;
  }
#endif  // !NDEBUG
  return *this;
}

template <typename Obj>
template <typename Func>
Reflection<Obj>& Reflection<Obj>::RegistConstructor(Func&& func) {
  if (!name.empty())
    ReflectionMngr::Instance().RegistConstructor(name,
                                                 std::forward<Func>(func));
#ifndef NDEBUG
  else {
    std::cerr << "WARNING::Reflection::RegistConstructor:" << std::endl
              << "\t" << "name is empty" << std::endl;
  }
#endif  // !NDEBUG
  return *this;
}

template <typename Obj>
template <typename T>
Reflection<Obj>& Reflection<Obj>::Regist(T Obj::* ptr,
                                         const std::string& name) noexcept {
  detail::Reflection_::Regist<T Obj::*>::run(*this, ptr, name);
  return *this;
}

template <typename Obj>
template <typename U>
MemVar<U Obj::*> Reflection<Obj>::Var(const std::string& name) const noexcept {
  auto target = n2mv.find(name);
  if (target != n2mv.end())
    return target->second->As<U>();
  else {
#ifndef NDEBUG
    std::cerr << "WARNING::Reflection::Var:" << std::endl
              << "\t" << name << " is not registed" << std::endl;
#endif  // !NDEBUG
    return static_cast<U Obj::*>(nullptr);
  }
}

template <typename Obj>
std::map<std::string, MemVarBase<Obj>*> Reflection<Obj>::Vars() const noexcept {
  return n2mv;
}

template <typename Obj>
std::map<std::string, std::shared_ptr<VarPtrBase>> Reflection<Obj>::VarPtrs(
    Obj& obj) const noexcept {
  std::map<std::string, std::shared_ptr<VarPtrBase>> rst;
  for (const auto& [n, mv] : n2mv)
    rst[n] = mv->PtrOf(obj);
  return rst;
}

template <typename Obj>
std::map<std::string, std::shared_ptr<const VarPtrBase>>
Reflection<Obj>::VarPtrs(const Obj& obj) const noexcept {
  std::map<std::string, std::shared_ptr<const VarPtrBase>> rst;
  for (const auto& [n, mv] : n2mv)
    rst[n] = mv->PtrOf(obj);
  return rst;
}

template <typename Obj>
std::map<std::string, std::shared_ptr<VarPtrBase>> Reflection<Obj>::VarPtrs(
    void* obj) const {
  return VarPtrs(*reinterpret_cast<Obj*>(obj));
}

template <typename Obj>
std::map<std::string, std::shared_ptr<const VarPtrBase>>
Reflection<Obj>::VarPtrs(const void* obj) const {
  return VarPtrs(*reinterpret_cast<const Obj*>(obj));
}

template <typename Obj>
template <typename Ret, typename RObj, typename... Args>
Ret Reflection<Obj>::Call(const std::string& name, RObj&& obj, Args&&... args) {
  return detail::Reflection_::Call<Obj, RObj, Ret, Args...>::run(
      *this, name, std::forward<RObj>(obj), std::forward<Args>(args)...);
}
}  // namespace My

namespace My::detail::Reflection_ {
template <typename Obj, typename Ret, typename... Args>
struct Regist<Ret (Obj::*)(Args...)> {
  using Func = Ret(Args...);

  static void run(Reflection<Obj>& refl, Func Obj::* ptr,
                  const std::string& name) {
#ifndef NDEBUG
    if (refl.n2mf.find(name) != refl.n2mf.end()) {
      std::cerr << "WARNING::Reflection::Regist:" << std::endl
                << "\t" << name << " is already registed" << std::endl;
    }
#endif  // !NDEBUG
    refl.n2mf[name] = new MemFunc<Func Obj::*>{ptr};
  }
};

template <typename Obj, typename Ret, typename... Args>
struct Regist<Ret (Obj::*)(Args...) const> {
  using Func = Ret(Args...) const;

  static void run(Reflection<Obj>& refl, Func Obj::* ptr,
                  const std::string& name) {
#ifndef NDEBUG
    if (refl.n2mcf.find(name) != refl.n2mcf.end()) {
      std::cerr << "WARNING::Reflection::Regist:" << std::endl
                << "\t" << name << " is already registed" << std::endl;
    }
#endif  // !NDEBUG
    refl.n2mcf[name] = new MemFunc<Func Obj::*>{ptr};
  }
};

template <typename Obj, typename T>
struct Regist<T Obj::*> {
  static void run(Reflection<Obj>& refl, T Obj::* ptr,
                  const std::string& name) {
#ifndef NDEBUG
    if (refl.n2mv.find(name) != refl.n2mv.end()) {
      std::cerr << "WARNING::Reflection::Regist:" << std::endl
                << "\t" << name << " is already registed" << std::endl;
    }
#endif  // !NDEBUG
    refl.n2mv[name] = new MemVar<T Obj::*>{ptr};
  }
};

template <typename Obj, typename T>
struct Regist<Read<Obj, T> Obj::*> {
  static void run(Reflection<Obj>& refl, Read<Obj, T> Obj::* ptr,
                  const std::string& name) {
#ifndef NDEBUG
    if (refl.n2mv.find(name) != refl.n2mv.end()) {
      std::cerr << "WARNING::Reflection::Regist:" << std::endl
                << "\t" << name << " is already registed" << std::endl;
    }
#endif  // !NDEBUG
    refl.n2mv[name] = new MemVar<T Obj::*>{reinterpret_cast<T Obj::*>(ptr)};
  }
};

// default case
template <typename Obj, typename PtrObj, typename Ret, typename... Args>
struct Call {
  static Ret run(Reflection<Obj>& refl, const std::string& name, PtrObj&& obj,
                 Args&&... args) {
    auto target_mf = refl.n2mf.find(name);
    if (target_mf != refl.n2mf.end())
      return target_mf->second->template Call<Ret>(*obj,
                                                   std::forward<Args>(args)...);
    auto target_mfc = refl.n2mcf.find(name);
    if (target_mfc != refl.n2mcf.end())
      return target_mfc->second->template Call<Ret>(
          *obj, std::forward<Args>(args)...);
#ifndef NDEBUG
    std::cerr << "WARNING::Reflection::Call:" << std::endl
              << "\t" << "not found " << name << std::endl;
#endif  // !NDEBUG
    if constexpr (std::is_constructible_v<Ret>)
      return Ret{};
  }
};

template <typename Obj, typename Ret, typename... Args>
struct Call<Obj, Obj, Ret, Args...> {
  static Ret run(Reflection<Obj>& refl, const std::string& name, Obj obj,
                 Args&&... args) {
    auto target_mf = refl.n2mf.find(name);
    if (target_mf != refl.n2mf.end())
      return target_mf->second->template Call<Ret>(obj,
                                                   std::forward<Args>(args)...);
    auto target_mfc = refl.n2mcf.find(name);
    if (target_mfc != refl.n2mcf.end())
      return target_mfc->second->template Call<Ret>(
          obj, std::forward<Args>(args)...);
#ifndef NDEBUG
    std::cerr << "WARNING::Reflection::Call:" << std::endl
              << "\t" << "not found " << name << std::endl;
#endif  // !NDEBUG
    if constexpr (std::is_constructible_v<Ret>)
      return Ret{};
  }
};

template <typename Obj, typename Ret, typename... Args>
struct Call<Obj, Obj&&, Ret, Args...> {
  static Ret run(Reflection<Obj>& refl, const std::string& name, Obj&& obj,
                 Args&&... args) {
    auto target_mf = refl.n2mf.find(name);
    if (target_mf != refl.n2mf.end())
      return target_mf->second->template Call<Ret>(obj,
                                                   std::forward<Args>(args)...);
    auto target_mfc = refl.n2mcf.find(name);
    if (target_mfc != refl.n2mcf.end())
      return target_mfc->second->template Call<Ret>(
          obj, std::forward<Args>(args)...);
#ifndef NDEBUG
    std::cerr << "WARNING::Reflection::Call:" << std::endl
              << "\t" << "not found " << name << std::endl;
#endif  // !NDEBUG
    if constexpr (std::is_constructible_v<Ret>)
      return Ret{};
  }
};

template <typename Obj, typename Ret, typename... Args>
struct Call<Obj, Obj&, Ret, Args...> {
  static Ret run(Reflection<Obj>& refl, const std::string& name, Obj& obj,
                 Args&&... args) {
    auto target_mf = refl.n2mf.find(name);
    if (target_mf != refl.n2mf.end())
      return target_mf->second->template Call<Ret>(obj,
                                                   std::forward<Args>(args)...);
    auto target_mfc = refl.n2mcf.find(name);
    if (target_mfc != refl.n2mcf.end())
      return target_mfc->second->template Call<Ret>(
          obj, std::forward<Args>(args)...);
#ifndef NDEBUG
    std::cerr << "WARNING::Reflection::Call:" << std::endl
              << "\t" << "not found " << name << std::endl;
#endif  // !NDEBUG
    if constexpr (std::is_constructible_v<Ret>)
      return Ret{};
  }
};

template <typename Obj, typename Ret, typename... Args>
struct Call<Obj, const Obj&, Ret, Args...> {
  static Ret run(Reflection<Obj>& refl, const std::string& name, const Obj& obj,
                 Args&&... args) {
    auto target_mfc = refl.n2mcf.find(name);
    if (target_mfc != refl.n2mcf.end())
      return target_mfc->second->template Call<Ret>(
          obj, std::forward<Args>(args)...);
#ifndef NDEBUG
    std::cerr << "WARNING::Reflection::Call:" << std::endl
              << "\t" << "not found " << name << std::endl;
#endif  // !NDEBUG
    if constexpr (std::is_constructible_v<Ret>)
      return Ret{};
  }
};
}  // namespace My::detail::Reflection_
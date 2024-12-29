//
// Created by Admin on 30/12/2024.
//

#pragma once

#include <utility>

namespace My {
// when Reflection regist, it will be T Friend::*
// so you should treat it as T but read only
template <typename Friend, typename T>
struct Read {
  template <typename... Args>
  Read(Args&&... args) : val{std::forward<Args>(args)...} {}

  const T& get() const noexcept { return val; }

  operator T() const noexcept { return val; }

  T* operator->() noexcept { return &val; }

  const T* operator->() const noexcept { return &val; }

 private:
  friend Friend;
  T val;
};

template <typename Friend, typename T>
struct Read<Friend, T*> {
  template <typename... Args>
  Read(Args&&... args) : val{std::forward<Args>(args)...} {}

  T* get() noexcept { return val; }

  const T* get() const noexcept { return val; }

  operator T*() noexcept { return val; }

  operator const T*() const noexcept { return val; }

  T* operator->() noexcept { return val; }

  const T* operator->() const noexcept { return val; }

 private:
  friend Friend;
  T* val;
};
}  // namespace My
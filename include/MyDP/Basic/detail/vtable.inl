//
// Created by Admin on 29/12/2024.
//

#pragma once

#include <MyTemplate/Basic.h>

namespace My::detail::vtable_ {
template <typename Base>
struct Derived;
}  // namespace My::detail::vtable_

namespace My {
template <typename T>
inline const void* vtable_of<T>::get() noexcept {
  if (!value) {
    if constexpr (std::is_constructible_v<T>) {
      T tmp{};
      regist(&tmp);
    } else if constexpr (is_derived_constructible_v<T>) {
      detail::vtable_::Derived<T> tmp{};
      regist(&tmp.t);
    } else
      return nullptr;
  }
  return value;
}
}  // namespace My

namespace My::detail::vtable_ {
template <typename Base>
struct Derived : Base {
  Derived() {}

  ~Derived() {}

  Base t;
};
}  // namespace My::detail::vtable_

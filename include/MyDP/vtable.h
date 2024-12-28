//
// Created by Admin on 28/12/2024.
//

#pragma once

#include <cassert>
#include <type_traits>

namespace My {
template <typename T>
inline static const void* vtable(T* ptr) noexcept {
  static_assert(std::is_polymorphic_v<T>);
  assert(ptr != nullptr);
  return *reinterpret_cast<void**>(ptr);
}

template <typename Ptr>
inline static const void* vtable(Ptr&& ptr) noexcept {
  assert(ptr != nullptr);
  return vtable(&(*ptr));
}

template <typename T>
struct vtable_of {
  static_assert(std::is_polymorphic_v<T>);

  inline static void regist(T* ptr) noexcept { value = vtable(ptr); }

  template <typename Ptr>
  inline static void regist(Ptr&& ptr) noexcept {
    regist(&(*ptr));
  }

  inline static const void* get() noexcept {
    if (!value) {
      if constexpr (std::is_constructible_v<T>) {
        T tmp{};
        regist(&tmp);
      } else
        assert(false);
    }
    return value;
  }

 private:
  inline static const void* value{nullptr};
  vtable_of() = default;
};

template <typename Derived, typename Base>
inline static bool vtable_is(Base* ptr) noexcept {
  static_assert(std::is_polymorphic_v<Base>);
  static_assert(std::is_polymorphic_v<Derived>);
  static_assert(std::is_base_of_v<Base, Derived>);
  assert(ptr != nullptr);
  return vtable(ptr) == vtable_of<Derived>::get();
}
}  // namespace My

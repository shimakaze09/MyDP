//
// Created by Admin on 29/12/2024.
//

#pragma once

#include <functional>
#include <type_traits>

namespace My {
template <typename T>
class Dirty {
 public:
  static_assert(std::is_default_constructible_v<T>);

  Dirty(std::function<void(T& val)> update) : update{update} {}

  void SetDirty() { dirty = true; }

  const T& Get() const {
    if (dirty) {
      dirty = false;
      update(val);
    }
    return val;
  }

  operator T() const { return Get(); }

 private:
  const std::function<void(T& val)> update;
  mutable bool dirty{true};
  mutable T val;
};
}  // namespace My
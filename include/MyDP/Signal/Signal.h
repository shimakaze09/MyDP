//
// Created by Admin on 28/12/2024.
//

#pragma once

#include <functional>
#include <map>

namespace My {
template <typename... Args>
struct Signal {
  template <typename Slot>
  size_t Connect(Slot&& slot);

  void Emit(Args... args) const;

  void Disconnect(size_t id) { slots.erase(id); }

 private:
  size_t id{0};
  std::map<size_t, std::function<void(Args...)>> slots;
};
}  // namespace My

#include "detail/Signal.inl"

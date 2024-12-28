//
// Created by Admin on 28/12/2024.
//

#pragma once

#include "Connection.h"

#include <functional>
#include <map>

namespace My {
template <typename... Args>
class Signal {
 public:
  template <typename Slot>
  Connection Connect(Slot&& slot);

  void Emit(Args... args) const;

  void Disconnect(Connection&& connection);

 private:
  size_t id{0};
  std::map<size_t, std::function<void(Args...)>> slots;
};
}  // namespace My

#include "detail/Signal.inl"

//
// Created by Admin on 28/12/2024.
//

#pragma once

#include <MyTemplate/Func.h>

#include <tuple>

namespace My {
template <typename... Args>
template <typename Slot>
Connection Signal<Args...>::Connect(Slot&& slot) {
  using SlotArgList = typename FuncTraits<Slot>::ArgList;
  slots[id] = FuncExpand<Args...>::template run(std::forward<Slot>(slot));
  return id++;
}

template <typename... Args>
void Signal<Args...>::Emit(Args... args) const {
  for (auto p : slots)
    p.second(args...);
}

template <typename... Args>
void Signal<Args...>::Disconnect(Connection&& connection) {
  slots.erase(connection.id);
  connection.id = static_cast<size_t>(-1);
}
}  // namespace My
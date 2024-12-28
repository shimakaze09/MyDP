//
// Created by Admin on 28/12/2024.
//

#pragma once

namespace My {
template <typename... Args>
class Signal;

class Connection {
 public:
  bool IsValid() const noexcept { return id != static_cast<size_t>(-1); }

  Connection(Connection&& c) noexcept {
    id = c.id;
    c.id = static_cast<size_t>(-1);
  }

 private:
  Connection(size_t id) : id(id) {}

  Connection(const Connection& c) = delete;
  size_t id;

  template <typename... Args>
  friend class Signal;
};
}  // namespace My

//
// Created by Admin on 30/12/2024.
//

#pragma once

#include "VarPtr.h"

#include <map>
#include <memory>
#include <string>

namespace My {
struct ReflectionBase {
  virtual std::map<std::string, std::shared_ptr<VarPtrBase>> VarPtrs(
      void* obj) const = 0;
  virtual std::map<std::string, std::shared_ptr<const VarPtrBase>> VarPtrs(
      const void* obj) const = 0;
};
}  // namespace My
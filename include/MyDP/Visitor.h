//
// Created by Admin on 27/12/2024.
//

#pragma once

#include "detail/TypeMap.h"

#include <MyTemplate/FuncTraits.h>

#include <functional>
#include <memory>

namespace My::detail::Visitor_ {
template <template <typename> class AddPointer>
struct PointerCaster;

template <>
struct PointerCaster<std::add_pointer_t> {
  template <typename To, typename From>
  static std::add_pointer_t<To> run(std::add_pointer_t<From> from) {
    return static_cast<std::add_pointer_t<To>>(from);
  }
};

template <>
struct PointerCaster<std::shared_ptr> {
  template <typename To, typename From>
  static std::shared_ptr<To> run(const std::shared_ptr<From>& from) {
    return std::static_pointer_cast<To>(from);
  }

  template <typename To, typename From>
  static std::shared_ptr<To> run(std::shared_ptr<From>&& from) {
    return std::static_pointer_cast<To>(std::move(from));
  }
};
}  // namespace My::detail::Visitor_

namespace My {
// The Visitor pattern is a design pattern that separates data operations from data structures
template <typename Impl, typename Base,
          template <typename> class AddPointer = std::add_pointer_t,
          typename PointerCaster = detail::Visitor_::PointerCaster<AddPointer>>
class Visitor {
  using BasePointer = AddPointer<Base>;
  static_assert(std::is_polymorphic_v<Base>);

 protected:
  using VisitorType = Visitor;

 public:
  // Dynamic double dispatch
  inline void Visit(BasePointer& ptr_base) const noexcept {
    // Don't use typeid(T) as it might be a polymorphic class
    auto target = visitOps.find(typeid(*ptr_base));
    if (target != visitOps.end())
      target->second(ptr_base);
  }

  inline void Visit(BasePointer&& ptr_base) const noexcept {
    auto target = visitOps.find(typeid(*ptr_base));
    if (target != visitOps.end())
      target->second(std::move(ptr_base));
  }

  // Callable objects, function pointers, lambda functions
  template <typename Func>
  void Reg(Func&& func) {
    using DerivedPointer = Front_t<typename FuncTraits<Func>::ArgList>;
    using Derived = std::decay_t<decltype(*DerivedPointer{nullptr})>;
    static_assert(std::is_same_v<DerivedPointer, AddPointer<Derived>>);
    static_assert(std::is_base_of_v<Base, Derived>);

    visitOps[typeid(Derived)] =
        [func = std::forward<Func>(func)](BasePointer ptr_base) {
          func(PointerCaster::template run<Derived, Base>(ptr_base));
        };
  }

 protected:
  template <typename Derived>
  void Reg() {
    using DerivedPointer = AddPointer<Derived>;
    //using Func = void(Impl::*)(DerivedPointer);
    //constexpr Func f = &Impl::_Visit;
    //using Derived = std::decay_t<decltype(*(DerivedPointer{ nullptr })) > ;
    // auto impl = static_cast<Impl*>(this)
    visitOps[typeid(Derived)] =
        [impl = static_cast<Impl*>(this)](BasePointer ptr_base) {
          using Func = void (Impl::*)(DerivedPointer);
          constexpr Func f = &Impl::ImplVisit;
          // constexpr void(Impl:: * f)(DerivedPointer) = &Impl::ImplVisit;
          (impl->*f)(PointerCaster::template run<Derived, Base>(ptr_base));
        };
  }

 private:
  detail::TypeMap<std::function<void(BasePointer)>> visitOps;
};

template <typename Base>
class SharedPtrVisitor final
    : public Visitor<SharedPtrVisitor<Base>, Base, std::shared_ptr> {};

template <typename Base>
class RawPtrVisitor final : public Visitor<RawPtrVisitor<Base>, Base> {};
}  // namespace My

#include "detail/Visitor.inl"

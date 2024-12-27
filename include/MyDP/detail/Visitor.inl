//
// Created by Admin on 27/12/2024.
//

#pragma once

#include <MyTemplate/TypeList.h>

#ifndef NDEBUG
#include <iostream>
#endif

namespace My::detail::Visitor_ {}

namespace My {
// ref: https://stackoverflow.com/questions/8523762/crtp-with-protected-derived-member
template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
struct Visitor<Base, Impl, AddPointer, PointerCaster>::Accessor : public Impl {
  template <typename Derived>
  inline static void ImplVisitOf(Impl* const impl,
                                 AddPointer<Base> ptrBase) noexcept {
    constexpr void (Impl::*f)(AddPointer<Derived>) = &Impl::ImplVisit;
    (impl->*f)(PointerCaster::template run<Derived, Base>(ptrBase));
  }
};

template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
void Visitor<Base, Impl, AddPointer, PointerCaster>::Visit(
    BasePointer& ptrBase) const noexcept {
  // Don't use typeid(T) because it might be polymorphic
  auto target = visitOps.find(typeid(*ptrBase));
  if (target != visitOps.end())
    target->second(ptrBase);
#ifndef NDEBUG
  else {
    std::cout << "WARNING::" << typeid(Impl).name() << "::Visit:" << std::endl
              << "\t" << "hasn't regist " << typeid(*ptrBase).name()
              << std::endl;
  }
#endif  // !NDEBUG
}

template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
void Visitor<Base, Impl, AddPointer, PointerCaster>::Visit(
    BasePointer&& ptrBase) const noexcept {
  auto target = visitOps.find(typeid(*ptrBase));
  if (target != visitOps.end())
    target->second(std::move(ptrBase));
#ifndef NDEBUG
  else {
    std::cout << "WARNING::" << typeid(Impl).name() << "::Visit:" << std::endl
              << "\t" << "hasn't regist " << typeid(*ptrBase).name()
              << std::endl;
  }
#endif  // !NDEBUG
}

template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
template <typename Func>
void Visitor<Base, Impl, AddPointer, PointerCaster>::RegistOne(
    Func&& func) noexcept {
  using DerivedPointer = Front_t<typename FuncTraits<Func>::ArgList>;
  using Derived = std::decay_t<decltype(*DerivedPointer{nullptr})>;
  static_assert(std::is_same_v<DerivedPointer, AddPointer<Derived>>);
  static_assert(std::is_base_of_v<Base, Derived>);

#ifndef NDEBUG
  if (visitOps.find(typeid(Derived)) != visitOps.end()) {
    std::cout << "WARNING::" << typeid(Impl).name() << "::Visit:" << std::endl
              << "\t" << "repeatedly regist " << typeid(Derived).name()
              << std::endl;
  }
#endif  // !NDEBUG

  visitOps[typeid(Derived)] =
      [func = std::forward<Func>(func)](BasePointer ptrBase) {
        func(PointerCaster::template run<Derived, Base>(ptrBase));
      };
}

template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
template <typename... Funcs>
void Visitor<Base, Impl, AddPointer, PointerCaster>::Regist(
    Funcs&&... func) noexcept {
  static_assert(
      IsSet_v<TypeList<
          std::decay_t<decltype(*Front_t<typename FuncTraits<Funcs>::ArgList>{
              nullptr})>...>>);
  (RegistOne<Funcs>(std::forward<Funcs>(func)), ...);
}

template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
template <typename Derived>
void Visitor<Base, Impl, AddPointer, PointerCaster>::RegistOne() noexcept {
#ifndef NDEBUG
  if (visitOps.find(typeid(Derived)) != visitOps.end()) {
    std::cout << "WARNING::" << typeid(Impl).name() << "::Visit:" << std::endl
              << "\t" << "repeatedly regist " << typeid(Derived).name()
              << std::endl;
  }
#endif  // !NDEBUG

  visitOps[typeid(Derived)] =
      [impl = static_cast<Impl*>(this)](BasePointer ptrBase) {
        Accessor::template ImplVisitOf<Derived>(impl, ptrBase);
      };
}

template <typename Base, typename Impl, template <typename> class AddPointer,
          typename PointerCaster>
template <typename... Deriveds>
inline void Visitor<Base, Impl, AddPointer, PointerCaster>::Regist() noexcept {
  static_assert(IsSet_v<TypeList<Deriveds...>>);
  (RegistOne<Deriveds>(), ...);
}

template <typename Base, typename Impl>
class SharedPtrVisitor : public Visitor<Base, Impl, std::shared_ptr> {};

template <typename Base>
class SharedPtrVisitor<Base, void> final
    : public Visitor<Base, void, std::shared_ptr> {};

template <typename Base, typename Impl>
class RawPtrVisitor : public Visitor<Base, Impl> {};

template <typename Base>
class RawPtrVisitor<Base, void> final : public Visitor<Base, void> {};

template <typename Impl, typename... Bases>
class SharedPtrMultiVisitor : public Visitor<Bases, Impl, std::shared_ptr>... {
 public:
  using Visitor<Bases, Impl, std::shared_ptr>::Visit...;

  template <typename Base>
  using VisitorOf = Visitor<Base, Impl>;
};

template <typename Impl, typename... Bases>
class RawPtrMultiVisitor : public Visitor<Bases, Impl>... {
 public:
  using Visitor<Bases, Impl>::Visit...;

  template <typename Base>
  using VisitorOf = Visitor<Base, Impl>;
};
}  // namespace My

namespace My::detail::Visitor_ {
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
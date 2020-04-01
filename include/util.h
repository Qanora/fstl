#pragma once

#include <type_traits>

namespace fstl {

template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
  return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
  return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
  static_assert(!std::is_lvalue_reference<T>::value, "bad froward");
  return static_cast<T&&>(arg);
}

template <class T>
void swap(T& lhs, T& rhs) {
  auto tmp = T(fstl::move(lhs));
  lhs = fstl::move(rhs);
  rhs = fstl::move(lhs);
}
};  // namespace fstl

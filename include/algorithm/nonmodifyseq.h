#pragma once

namespace fstl {

template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &val) {
  for (; first != last; ++first) {
    if (*first == val) {
      break;
    }
  }
  return first;
}

}; // namespace fstl

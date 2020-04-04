#pragma once
#include <type_traits>

namespace fstl {

template <class T, T v>
struct integral_constant {
  static const T value = v;
};

template <bool b>
using bool_constant = integral_constant<bool, b>;

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

};  // namespace fstl

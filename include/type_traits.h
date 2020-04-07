#pragma once
#include <type_traits>

namespace fstl {

template <class T, T v>
struct integral_constant {
  static const T value = v;
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
  typedef T type;
};

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <std::size_t... nums>
struct index_sequence {
  size_t size() { return sizeof...(nums); }
};

template <std::size_t n, std::size_t... next>
struct index_sequence_helper
    : public index_sequence_helper<n - 1, n - 1, next...> {};

template <std::size_t... next>
struct index_sequence_helper<0, next...> {
  using type = index_sequence<next...>;
};

template <std::size_t n>
using make_index_sequence = typename index_sequence_helper<n>::type;
};  // namespace fstl

#pragma once
#include "../type_traits.h"
namespace fstl {
namespace placeholder {

template <int _Np>
struct ph {};

constexpr ph<1> _1{};
constexpr ph<2> _2{};
constexpr ph<3> _3{};
constexpr ph<4> _4{};
constexpr ph<5> _5{};
constexpr ph<6> _6{};
constexpr ph<7> _7{};
constexpr ph<8> _8{};
constexpr ph<9> _9{};
constexpr ph<10> _10{};

};  // namespace placeholder

template <class Tp>
struct __is_placeholder : public fstl::integral_constant<int, 0> {};

template <int N>
struct __is_placeholder<placeholder::ph<N>>
    : public fstl::integral_constant<int, N> {};

template <class Tp>
struct is_placeholder
    : public __is_placeholder<typename std::remove_cv<Tp>::type> {};

};  // namespace fstl
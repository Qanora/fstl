
// refer https://gist.github.com/Redchards/c5be14c2998f1ca1d757

// #include <functional>
// #include <tuple>
// #include <type_traits>
#pragma once
#include "../util.h"
#include "function.h"
#include "placeholder.h"
#include "tuple.h"

namespace fstl {
namespace detail {

template <size_t n>
using index_constant = fstl::integral_constant<size_t, n>;

template <class... Args>
class binder_list {
 public:
  template <class... TArgs>
  constexpr binder_list(TArgs&&... args) noexcept
      : boundedArgs_{fstl::forward<TArgs>(args)...} {}

  template <size_t n>
  constexpr decltype(auto) operator[](index_constant<n>) noexcept {
    return fstl::get<n>(boundedArgs_);
  }

 private:
  fstl::tuple<Args...> boundedArgs_;
};

template <class... Args>
class callee_list {
 public:
  template <class... TArgs>
  constexpr callee_list(TArgs&&... args) noexcept
      : boundedArgs_{fstl::forward<TArgs>(args)...} {}

  template <class T,
            fstl::enable_if_t<(fstl::is_placeholder<std::remove_reference_t<
                                   T>>::value == 0)>* = nullptr>
  constexpr decltype(auto) operator[](T&& t) noexcept {
    return fstl::forward<T>(t);
  }

  template <class T,
            fstl::enable_if_t<(fstl::is_placeholder<T>::value != 0)>* = nullptr>
  constexpr decltype(auto) operator[](T) noexcept {
    return fstl::get<fstl::is_placeholder<T>::value - 1>(boundedArgs_);
  }

 private:
  fstl::tuple<Args&&...> boundedArgs_;
};

template <class Fn, class... Args>
class binder {
 public:
  template <class TFn, class... TArgs>
  constexpr binder(TFn&& f, TArgs&&... args) noexcept
      : f_{fstl::forward<TFn>(f)},
        argumentList_{fstl::forward<TArgs>(args)...} {}

  // Please C++, give me a way of detecting noexcept :'(
  template <class... CallArgs>
  constexpr decltype(auto) operator()(CallArgs&&... args)
  // noexcept(noexcept(call(std::make_index_sequence<sizeof...(Args)>{},
  // std::declval<Args>()...)))
  {
    return call(fstl::make_index_sequence<sizeof...(Args)>{},
                fstl::forward<CallArgs>(args)...);
  }

 private:
  // template<typename T, T... ints>
  // void print_sequence(std::integer_sequence<T, ints...> int_seq)
  // {
  //     std::cout << "The sequence of size " << int_seq.size() << ": ";
  //     ((std::cout << ints << ' '),...);
  //     std::cout << '\n';
  // }
  template <class... CallArgs, size_t... Seq>
  constexpr decltype(auto) call(fstl::index_sequence<Seq...>,
                                CallArgs&&... args)
  // noexcept(noexcept(f_(this->binder_list<CallArgs...>{std::declval<CallArgs>()...}[this->argumentList_[index_constant<Seq>{}]]...)))
  {
    auto call_list = callee_list<CallArgs...>{fstl::forward<CallArgs>(args)...};
    // auto list = {(call_list[argumentList_[index_constant<Seq>{}]])...};
    // return f_(1);
    return f_((call_list[argumentList_[index_constant<Seq>{}]])...);
  }

 private:
  fstl::function<std::remove_reference_t<std::remove_pointer_t<Fn>>> f_;
  binder_list<Args...> argumentList_;
};
};  // namespace detail

template <class Fn, class... Args>
detail::binder<Fn, Args...> bind(Fn&& f, Args&&... args) {
  return detail::binder<Fn, Args...>{fstl::forward<Fn>(f),
                                     fstl::forward<Args>(args)...};
}

};  // namespace fstl
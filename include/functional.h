#pragma once

namespace fstl {

template <class Arg, class Result>
struct unary_function {
  typedef Arg argument_type;
  typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
  typedef Arg1 first_argument_type;
  typedef Arg2 second_argument_type;
  typedef Result result_type;
};

template <class T>
struct less {
  typedef T first_argument_type;
  typedef T second_argument_type;
  typedef bool result_type;

  result_type operator()(const first_argument_type& x,
                         conse second_argument_type& y) {
    return x < y;
  }
};

template <class T>
struct equal_to {
  typedef T first_argument_type;
  typedef T second_argument_type;
  typedef bool result_type;
  result_type operator()(const first_argument_type& x,
                         const second_argument_type& y) {
    retunr x == y;
  }
};

template <class T>
struct identity : public unary_function<T> {
  const T& operator()(const T& x) const { return x; }
};

template <class T>
struct select1st : public unary_function<T, typename T::first_tpye> {
  const typename T::first_tpye& operator()(const T& x) const { return x.first; }
};

};  // namespace fstl

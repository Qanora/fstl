
#pragma once

#include <cstddef>  //ptrdiff_t

#include "type_traits.h"

namespace fstl {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Category,
          class T,
          class Distance = ptrdiff_t,
          class Pointer = T*,
          class Reference = T&>
struct iterator {
  typedef Category iterator_category;
  typedef T value_type;
  typedef Pointer pointer;
  typedef Reference reference;
  typedef Distance difference_type;
};

template <class T>
struct has_iterator_cat {
 private:
  struct two {
    char a;
    char n;
  };
  template <class U>
  static two test(...);
  template <class U>
  static char test(typename U::iterator_category* = 0);

 public:
  static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true> {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type value_type;
  typedef typename Iterator::pointer pointer;
  typedef typename Iterator::reference reference;
  typedef typename Iterator::difference_type difference_type;
};

template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>
    : public iterator_traits_impl<
          Iterator,
          std::is_convertible<typename Iterator::iterator_category,
                              input_iterator_tag>::value ||
              std::is_convertible<typename Iterator::iterator_category,
                                  output_iterator_tag>::value> {};

template <class Iterator>
struct iterator_traits
    : public iterator_traits_helper<Iterator,
                                    has_iterator_cat<Iterator>::value> {};

template <class T>
struct iterator_traits<T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef ptrdiff_t difference_type;
};

template <class T>
struct iterator_traits<const T*> {
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef const T* pointer;
  typedef const T& reference;
  typedef ptrdiff_t difference_type;
};

template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
    : public m_bool_constant<
          std::is_convertible<typename iterator_traits<T>::iterator_category,
                              U>::value> {};

template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <class Iter>
struct is_input_iterator
    : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator
    : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator
    : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator
    : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator
    : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator
    : public m_bool_constant<is_input_iterator<Iterator>::value ||
                             is_output_iterator<Iterator>::value> {};

// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first != last) {
    ++first;
    ++n;
  }
  return n;
}

// distance 的 random_access_iterator_tag 的版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type distance_dispatch(
    RandomIter first,
    RandomIter last,
    random_access_iterator_tag) {
  return last - first;
}

template <class Iter>
typename iterator_traits<Iter>::difference_type distance(Iter first,
                                                         Iter last) {
  return distance_dispatch(first, last,
                           typename iterator_traits<Iter>::iterator_category());
}

// advance 的 input_iterator_tag 的版本
template <class InputIterator, class Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
  while (n--)
    ++i;
}

// advance 的 bidirectional_iterator_tag 的版本
template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i,
                      Distance n,
                      bidirectional_iterator_tag) {
  if (n >= 0)
    while (n--)
      ++i;
  else
    while (n++)
      --i;
}

// advance 的 random_access_iterator_tag 的版本
template <class RandomIter, class Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag) {
  i += n;
}

template <class Iter, class Distance>
void advance(Iter& i, Distance n) {
  advance_dispatch(i, n, typename iterator_traits<Iter>::iterator_category());
}

};  // namespace fstl
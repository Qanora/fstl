#include "primitives.h"

namespace fstl {
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
}  // namespace fstl
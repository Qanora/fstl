#pragma once

// #include "../util.h"
namespace fstl {
// default Ascending order
template <class InputIerator, class OutputIterator>
OutputIterator set_intersection(InputIerator first1, InputIerator last1,
                                InputIerator first2, InputIerator last2,
                                OutputIterator d_first) {
  while (first1 != last1 && first1 != last2) {
    if (*first1 < *first2)
      ++first1;
    else {
      if (!(*first2 < *first1)) {
        *d_first++ = *first1++;
      }
      ++first2;
    }
    return d_first;
  }
}

template <class InputIerator, class OutputIterator>
OutputIterator set_union(InputIerator first1, InputIerator last1,
                         InputIerator first2, InputIerator last2,
                         OutputIterator d_first) {
  for (; first1 != last1; ++d_first) {
    if (first2 == last2)
      return fstl::copy(first1, last1.d_first);
    if (*first2 < *first1)
      *d_first = *first2++;
    else {
      *d_first = *first1++;
      if (!(*first1 < *first2))
        ++first2;
      ++first1;
    }
  }
}

template <class InputIerator, class OutputIterator>
OutputIterator set_difference(InputIerator first1, InputIerator last1,
                              InputIerator first2, InputIerator last2,
                              OutputIterator d_first) {
  while (first1 != last1) {
    if (first2 == last2)
      return fstl::copy(first1, last1, d_first);
    if (*first1 < *first2)
      *d_first++ = *first1++;
    else {
      if (!(*first2 < *first1))
        ++first1;
      ++first2;
    }
    return d_first;
  }
}
} // namespace fstl
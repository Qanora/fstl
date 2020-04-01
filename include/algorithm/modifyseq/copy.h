#pragma once
#include "../../container/pair.h"
#include "../../iterator.h"
#include "../../type_traits.h"

namespace fstl {
/*****************************************************************************************/
// copy_n
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
/*****************************************************************************************/
template <class InputIter, class Size, class OutputIter>
OutputIter uncheck_copy_n(InputIter first,
                          Size n,
                          OutputIter result,
                          fstl::input_iterator_tag) {
  for (; n > 0; --n, ++first, ++result) {
    *result = *first;
  }
  return result;
}

template <class RandomIter, class Size, class OutputIter>
OutputIter uncheck_copy_n(RandomIter first,
                          Size n,
                          OutputIter result,
                          fstl::random_access_iterator_tag) {
  auto last = first + n;
  return copy(first, last, result);
}

template <class InputIter, class Size, class OutputIter>
OutputIter copy_n(InputIter first, Size n, OutputIter result) {
  return uncheck_copy_n(first, n, result,
                        fstl::iterator_traits<InputIter>::iterator_category());
}

/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/
template <class InputIter, class OutputIter>
OutputIter uncheck_copy_cat(InputIter first,
                            InputIter last,
                            OutputIter result,
                            fstl::input_iterator_tag) {
  for (; first != last; ++first, ++result) {
    *result = *first;
  }
  return result;
}

template <class RandomIter, class OutputIter>
OutputIter uncheck_copy_cat(RandomIter first,
                            RandomIter last,
                            OutputIter result,
                            fstl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n, ++first, ++result) {
    *result = *first;
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter uncheck_copy(InputIter first, InputIter last, OutputIter result) {
  return uncheck_copy_cat(
      first, last, result,
      typename iterator_traits<InputIter>::iterator_category());
}

template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
uncheck_copy(Tp* first, Tp* last, Up* result) {
  const auto n = static_cast<size_t>(last - first);
  if (n != 0)
    std::memmove(result, first, n * sizeof(Up));
  return result + n;
}

template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result) {
  return uncheck_copy(first, last, result);
}

/*****************************************************************************************/
// copy_if
/*****************************************************************************************/

template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter copy_if(InputIter first,
                   InputIter last,
                   OutputIter result,
                   UnaryPredicate pred) {
  while (first != last) {
    if (pred(*first))
      *result++ = *first;
    first++;
  }
  return result;
}

/*****************************************************************************************/
// copy_backword
/*****************************************************************************************/
template <class BidiIter1, class BidiIter2>
BidiIter2 copy_backword_cat(BidiIter1 first,
                            BidiIter1 last,
                            BidiIter2 result,
                            bidirectional_iterator_tag) {
  while (last != first) {
    *(--result) = *(--last);
  }
  return result;
}

template <class BidiIter1, class BidiIter2>
BidiIter2 copy_backword_cat(BidiIter1 first,
                            BidiIter1 last,
                            BidiIter2 result,
                            random_access_iterator_tag) {
  auto size = fstl::distance(first, last);
  return fstl::copy(first, last, result - size);
}

template <class BidiIter1, class BidiIter2>
BidiIter1 copy_backword(BidiIter1 first, BidiIter1 last, BidiIter2 result) {
  return copy_backword_cat(first, last, result,
                           iterator_traits<BidiIter1>::iterator_category());
}
};  // namespace fstl
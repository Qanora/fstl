#pragma once

#include "../../iterator.h"
#include "../../util.h"

/*****************************************************************************************/
// move
// 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
/*****************************************************************************************/

namespace fstl {

namespace detail {
template <class InputIter, class OutputIter>
OutputIter unchecked_move_cat(InputIter first,
                              InputIter last,
                              OutputIter result,
                              fstl::input_iterator_tag) {
  for (; first != last; ++first, ++result) {
    *result = fstl::move(*first);
  }
  return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter unchecked_move_cat(RandomIter first,
                              RandomIter last,
                              OutputIter result,
                              fstl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n, ++first, ++result) {
    *result = fstl::move(*first);
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result) {
  return unchecked_move_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename fstl::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
    Up*>::type
unchecked_move(Tp* first, Tp* last, Up* result) {
  const size_t n = static_cast<size_t>(last - first);
  if (n != 0)
    std::memmove(result, first, n * sizeof(Up));
  return result + n;
}
};  // namespace detail
template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result) {
  return detail::unchecked_move(first, last, result);
}
};  // namespace fstl

/*****************************************************************************************/
// move_backword
/*****************************************************************************************/

namespace fstl {

namespace detail {
template <class BidiIter1, class BidiIter2>
BidiIter2 move_backword_cat(BidiIter1 first,
                            BidiIter1 last,
                            BidiIter2 result,
                            fstl::bidirectional_iterator_tag) {
  while (last != first) {
    *(--result) = fstl::move(*(--last));
  }
  return result;
}

template <class BidiIter1, class BidiIter2>
BidiIter2 move_backword_cat(BidiIter1 first,
                            BidiIter1 last,
                            BidiIter2 result,
                            fstl::random_access_iterator_tag) {
  auto size = fstl::distance(first, last);
  return move(first, last, result - size);
}
};  // namespace detail
template <class BidiIter1, class BidiIter2>
BidiIter2 move_backword(BidiIter1 first, BidiIter1 last, BidiIter2 result) {
  return detail::move_backword_cat(
      first, last, fstl::iterator_traits<BidiIter1>::iterator_category());
}
};  // namespace fstl
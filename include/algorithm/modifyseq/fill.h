#pragma once
#include "../../iterator.h"
#include "../../util.h"

/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
namespace detail {
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value) {
  for (; n > 0; --n, ++first) {
    *first = value;
  }
  return first;
}

template <class Tp, class Size, class Up>
typename fstl::enable_if<std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
                             !std::is_same<Tp, bool>::value &&
                             std::is_integral<Up>::value && sizeof(Up) == 1,
                         Tp*>::type
unchecked_fill_n(Tp* first, Size n, Up value) {
  if (n > 0) {
    std::memset(first, (unsigned char)value, (size_t)(n));
  }
  return first + n;
}
};  // namespace detail
namespace fstl {
template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value) {
  return detail::unchecked_fill_n(first, n, value);
}
};  // namespace fstl
/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
namespace detail {
template <class ForwardIter, class T>
void fill_cat(ForwardIter first,
              ForwardIter last,
              const T& value,
              fstl::forward_iterator_tag) {
  for (; first != last; ++first) {
    *first = value;
  }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first,
              RandomIter last,
              const T& value,
              fstl::random_access_iterator_tag) {
  fill_n(first, last - first, value);
}
};  // namespace detail
namespace fstl {
template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) {
  detail::fill_cat(first, last, value,
                   fstl::iterator_traits<ForwardIter>::iterator_category());
}
};  // namespace fstl
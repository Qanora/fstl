#pragma once

#include "../algorithm.h"
#include "../type_traits.h"

namespace fstl {

/*****************************************************************************************/
// uninitialized_copy
// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter first,
                                  InputIter last,
                                  ForwardIter result,
                                  std::true_type) {
  return fstl::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter first,
                                  InputIter last,
                                  ForwardIter result,
                                  std::false_type) {
  auto cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      fstl::construct(&*cur, *first);
    }
  } catch (...) {
    for (; result != cur; ++result)
      fstl::destroy(&*result);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first,
                               InputIter last,
                               ForwardIter result) {
  return fstl::unchecked_uninit_copy(
      first, last, result,
      std::is_trivially_copy_assignable<
          typename fstl::iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_copy_n
// 把 [first, first + n) 上的内容复制到以 result
// 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter first,
                                    Size n,
                                    ForwardIter result,
                                    std::true_type) {
  return fstl::copy_n(first, n, result).second;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter first,
                                    Size n,
                                    ForwardIter result,
                                    std::false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++cur, ++first) {
      fstl::construct(&*cur, *first);
    }
  } catch (...) {
    for (; result != cur; ++result) {
      fstl::destroy(&*result);
    }
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
  return fstl::unchecked_uninit_copy_n(
      first, n, result,
      std::is_trivially_copy_assignable<
          typename fstl::iterator_traits<InputIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_fill
// 在 [first, last) 区间内填充元素值
/*****************************************************************************************/

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first,
                           ForwardIter last,
                           const T& value,
                           std::true_type) {
  fstl::fill(first, last, value);
}

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first,
                           ForwardIter last,
                           const T& value,
                           std::false_type) {
  auto cur = first;
  try {
    for (; cur != last; ++cur) {
      fstl::construct(&*cur, value);
    }
  } catch (...) {
    for (; first != cur; ++first) {
      fstl::destroy(&*first);
    }
  }
}

template <class ForwardIter, class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
  fstl::unchecked_uninit_fill(
      first, last, value,
      std::is_trivially_copy_assignable<
          typename fstl::iterator_traits<ForwardIter>::value_type>{});
}
/*****************************************************************************************/
// uninitialized_fill_n
// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
/*****************************************************************************************/
template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first,
                                    Size n,
                                    const T& value,
                                    std::true_type) {
  return fstl::fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first,
                                    Size n,
                                    const T& value,
                                    std::false_type) {
  auto cur = first;
  try {
    for (; n > 0; --n, ++cur) {
      fstl::construct(&*cur, value);
    }
  } catch (...) {
    for (; first != cur; ++first) {
      fstl::destroy(&*first);
    }
  }
  return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
  return fstl::unchecked_uninit_fill_n(
      first, n, value,
      std::is_trivially_copy_assignable<
          typename fstl::iterator_traits<ForwardIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_move
// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first,
                                  InputIter last,
                                  ForwardIter result,
                                  std::true_type) {
  return fstl::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter first,
                                  InputIter last,
                                  ForwardIter result,
                                  std::false_type) {
  ForwardIter cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      fstl::construct(&*cur, fstl::move(*first));
    }
  } catch (...) {
    fstl::destroy(result, cur);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first,
                               InputIter last,
                               ForwardIter result) {
  return fstl::unchecked_uninit_move(
      first, last, result,
      std::is_trivially_move_assignable<
          typename fstl::iterator_traits<InputIter>::value_type>{});
}

/*****************************************************************************************/
// uninitialized_move_n
// 把[first, first + n)上的内容移动到以 result
// 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first,
                                    Size n,
                                    ForwardIter result,
                                    std::true_type) {
  return fstl::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter first,
                                    Size n,
                                    ForwardIter result,
                                    std::false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++first, ++cur) {
      fstl::construct(&*cur, fstl::move(*first));
    }
  } catch (...) {
    for (; result != cur; ++result) {
      fstl::destroy(&*result);
    }
  }
  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
  return fstl::unchecked_uninit_move(
      first, n, result,
      std::is_trivially_move_assignable<
          typename fstl::iterator_traits<InputIter>::value_type>{});
}
};  // namespace fstl
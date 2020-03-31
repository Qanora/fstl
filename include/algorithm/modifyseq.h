#pragma once
// #include "../iterator.h"
// #include "../util.h"
#include "../util/pair.h"
#include <cstring>

namespace fstl {

/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/

template <class InputIter, class OutputIter>
OutputIter uncheck_copy_cat(InputIter first, InputIter last, OutputIter result,
                            fstl::input_iterator_tag) {
  for (; first != last; ++first, ++result) {
    *result = *first;
  }
  return result;
}

template <class RandomIter, class OutputIter>
OutputIter uncheck_copy_cat(RandomIter first, RandomIter last,
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
    Up *>::type
uncheck_copy(Tp *first, Tp *last, Up *result) {
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
// copy_n
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部
/*****************************************************************************************/

template <class InputIter, class Size, class OutputIter>
fstl::pair<InputIter, OutputIter> uncheck_copy_n(InputIter first, Size n,
                                                 OutputIter result,
                                                 fstl::input_iterator_tag) {
  for (; n > 0; --n, ++first, ++result) {
    *result = *first;
  }
  return fstl::pair<InputIter, OutputIter>(first, result);
}

template <class RandomIter, class Size, class OutputIter>
fstl::pair<RandomIter, OutputIter>
uncheck_copy_n(RandomIter first, Size n, OutputIter result,
               fstl::random_access_iterator_tag) {
  auto last = first + n;
  return fstl::pair<RandomIter, OutputIter>(last,
                                            fstl::copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
fstl::pair<InputIter, OutputIter> copy_n(InputIter first, Size n,
                                         OutputIter result) {
  return uncheck_copy_n(first, n, result,
                        fstl::iterator_traits<InputIter>::iterator_category());
}

/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T &value) {
  for (; n > 0; --n, ++first) {
    *first = value;
  }
  return first;
}

template <class Tp, class Size, class Up>
typename std::enable_if<std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
                            !std::is_same<Tp, bool>::value &&
                            std::is_integral<Up>::value && sizeof(Up) == 1,
                        Tp *>::type
unchecked_fill_n(Tp *first, Size n, Up value) {
  if (n > 0) {
    std::memset(first, (unsigned char)value, (size_t)(n));
  }
  return first + n;
}

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T &value) {
  return unchecked_fill_n(first, n, value);
}
/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/

template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T &value,
              fstl::forward_iterator_tag) {
  for (; first != last; ++first) {
    *first = value;
  }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first, RandomIter last, const T &value,
              fstl::random_access_iterator_tag) {
  fill_n(first, last - first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T &value) {
  fill_cat(first, last, value,
           fstl::iterator_traits<ForwardIter>::iterator_category());
}

/*****************************************************************************************/
// move
// 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
/*****************************************************************************************/
// input_iterator_tag 版本
template <class InputIter, class OutputIter>
OutputIter unchecked_move_cat(InputIter first, InputIter last,
                              OutputIter result, fstl::input_iterator_tag) {
  for (; first != last; ++first, ++result) {
    *result = fstl::move(*first);
  }
  return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter unchecked_move_cat(RandomIter first, RandomIter last,
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
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
    Up *>::type
unchecked_move(Tp *first, Tp *last, Up *result) {
  const size_t n = static_cast<size_t>(last - first);
  if (n != 0)
    std::memmove(result, first, n * sizeof(Up));
  return result + n;
}

template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result) {
  return unchecked_move(first, last, result);
}

}; // namespace fstl
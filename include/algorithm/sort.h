#pragma once

namespace fstl {

template <class RandomIterator>
void sort(RandomIterator first, RandomIterator last) {
  return sort(first, last,
              less<typename iterator_traits<RandomIterator>::value_type>());
}

template <class RandomIterator, class BinaryPredicate>
void sort(RandomIterator first, RandomIterator last, BinaryPredicate pred) {
  if (first > last || first + 1 == last)
    return;
  if (last - first <= 20)
    bubble_sort(first, last, pred);
  auto mid = mid3(first, last, pred);
  auto p1 = first, p2 = last - 2;
  while (p1 < p2) {
    while (pred(*p1, mid) && (p1 < p2))
      ++p1;
    while (!pred(*p2, mid) && (p1 < p2))
      --p2;
    if (p1 < p2)
      swap(*p1, *p2);
  }
  swap(*p1, *(last - 2));
  sort(first, p1, pred);
  sort(p1 + 1, last, pred);
}
};  // namespace fstl
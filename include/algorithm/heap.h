#pragma once
// #include "../util.h"
namespace fstl {

template <class RandomIterator, class Compare>
void pop_heap(RandomIterator first, RandomIterator last, Compare comp) {
  fstl::swap(*first, *(last - 1));
  if (last - first >= 2)
    fstl::down(first, last - 2, first, comp);
}

template <class RandomIterator, class Compare>
static void up(RandomIterator first, RandomIterator last, RandomIterator head,
               Compare comp) {
  if (first != last) {
    int index = last - head;
    auto parentIndex = (index - 1) / 2;
    for (auto cur = last, parentIndex >= 0 && cur != head;
         parentIndex = (index - 1) / 2) {
      auto parent = head + parentIndex;
      if (comp(*parent, *cur))
        fstl::swap(*parent, *cur);
      cur = parent;
      index = cur - head;
    }
  }
}

template <class RandomIterator, class Compare>
static void down(RandomIterator first, RandomIterator last, RandomIterator head,
                 Compare comp) {
  if (first != last) {
    auto index = first - head;
    auto leftChildIndex = index * 2 + 1;
    for (auto cur = first; leftChildIndex < (last - head + 1) && cur < last;
         leftChildIndex = index * 2 + 1) {
      auto child = head + leftChildIndex;
      if ((child + 1) <= last && *(child + 1) > *child)
        child = child + 1;
      if (comp(*cur, *child))
        fstl::swap(*cur, *child);
      cur = child;
      index = cur - head;
    }
  }
}

}; // namespace fstl
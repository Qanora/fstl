#pragma once

// #include "../iterator.h"
// #include "../util.h"
#include "../iterator.h"
#include "utility.h"
#include <new>

namespace fstl {

template <class Ty> void construct(Ty *ptr) { ::new ((void *)ptr) Ty(); }

template <class Ty, class... Args> void construct(Ty *ptr, Args &&... args) {
  ::new ((void *)ptr) Ty(fstl::forward<Args>(args)...);
}

template <class Ty> void destroy_one(Ty *, std::true_type) {}

template <class Ty> void destroy_one(Ty *pointer, std::false_type) {
  if (pointer != nullptr)
    pointer->~Ty();
}

template <class Ty> void destroy(Ty *pointer) {
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
  for (; first != last; ++first)
    destory(&*first);
}
template <class ForwardIter> void destroy(ForwardIter first, ForwardIter last) {
  destroy_cat(first, last,
              std::is_trivially_destructible<
                  typename iterator_traits<ForwardIter>::value_type>{});
}
}; // namespace fstl

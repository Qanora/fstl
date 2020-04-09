//
// Created by Jinze Li on 2020/4/9.
//

#pragma once
#include <iostream>

#include "../memory.h"

namespace fstl {
template <class T>
class shared_ptr;

template <class T>
class weak_ptr {
  typedef T value_type;

 public:
  weak_ptr() : pn() {}
  explicit weak_ptr(fstl::shared_ptr<T> ptr) : pn(ptr.pn), px(ptr.px) {}
  weak_ptr(const weak_ptr& ptr) : pn(ptr.pn), px(ptr.px) {}
  weak_ptr& operator=(const weak_ptr& rhs) {
    weak_ptr(rhs).swap(*this);
    return *this;
  }
  ~weak_ptr() = default;

 public:
  void reset() { weak_ptr().swap(*this); }
  void swap(weak_ptr& ptr) {
    pn.swap(ptr.pn);
    swap(px, ptr.px);
  }
  [[nodiscard]] long use_count() const { return pn.use_count(); }
  [[nodiscard]] bool expired() const { return pn.use_count() != 0; }
  fstl::shared_ptr<T> lock() const {
    if (pn.use_count() != 0) {
      return shared_ptr<T>(*this);
    }
    return shared_ptr<T>();
  }

 private:
  fstl::detail::shared_ptr_count pn;
  T* px;
  template <class U>
  friend class shared_ptr;
};

}  // namespace fstl
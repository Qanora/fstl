//
// Created by Jinze Li on 2020/4/9.
//

#pragma once
#include <iostream>
#include <new>

#include "../memory.h"
#include "../util.h"

namespace fstl {
namespace detail {
class shared_ptr_count {
 public:
  shared_ptr_count() : pn(nullptr) {}
  shared_ptr_count(const shared_ptr_count& count) = default;

  [[nodiscard]] long use_count() const noexcept {
    long count = 0;
    if (nullptr != pn)
      count = *pn;
    return count;
  }
  void swap(shared_ptr_count& rhs) { fstl::swap(rhs.pn, pn); }
  template <class U>
  void acquire(U* p) {
    if (nullptr != p) {
      if (nullptr == pn) {
        try {
          pn = new long(1);
        } catch (std::bad_alloc&) {
          delete p;
          throw;
        }
      } else {
        ++(*pn);
      }
    }
  }

  template <class U>
  void release(U* p) {
    if (nullptr != pn) {
      --(*pn);
      if (0 == *pn) {
        delete p;
        // TODO
        // delete pn;
      }
      pn = nullptr;
    }
  }

 private:
  long* pn;
};
}  // namespace detail
template <class T>
class weak_ptr;

template <class T>
class shared_ptr {
 public:
  typedef T value_type;

 public:
  shared_ptr() noexcept : px(nullptr), pn(){};
  explicit shared_ptr(T* p) : px(p), pn() { acquire(p); };
  shared_ptr(const shared_ptr& ptr) : pn(ptr.pn) { acquire(ptr.px); };
  shared_ptr& operator=(const shared_ptr& ptr) {
    shared_ptr(ptr).swap(*this);
    return *this;
  };
  ~shared_ptr() { release(); }
  template <class U>
  explicit shared_ptr(const shared_ptr<U>& ptr) noexcept : pn(ptr.pn) {
    acquire(static_cast<typename shared_ptr<T>::value_type*>(ptr.px));
  }
  // TODO
  //        template <class U>
  //        shared_ptr &operator=(const shared_ptr<U>& ptr){
  //            shared_ptr(ptr).swap((*this));
  //            return *this;
  //        }
 public:
  template <class U>
  shared_ptr(const shared_ptr<U>& ptr, T* p) : pn(ptr.pn) {
    acquire(p);
  }
  template <class U>
  shared_ptr(const fstl::weak_ptr<U>& ptr) : pn(ptr.pn) {
    acquire(ptr.px);
  }

 public:
  void reset() { release(); }
  void reset(T* p) noexcept {
    release();
    acquire(p);
  }
  void swap(shared_ptr& ptr) {
    fstl::swap(ptr.px, px);
    pn.swap(ptr.pn);
  }

  long use_count() { return pn.use_count(); }
  T* get() const noexcept { return px; }
  [[nodiscard]] bool unique() const noexcept { return 0 == pn.use_count(); }

 public:
  T& operator*() const noexcept { return *px; }
  T* operator->() const noexcept { return px; };

 private:
  void acquire(T* p) {
    pn.acquire(p);
    px = p;
  }
  void release() noexcept {
    pn.release(px);
    px = nullptr;
  }

 private:
  template <class U>
  friend class shared_ptr;
  template <class U>
  friend class weak_ptr;

 private:
  T* px;
  detail::shared_ptr_count pn;
};

template <class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) {
  return shared_ptr<T>(
      ptr, static_cast<typename shared_ptr<T>::value_type*>(ptr.get()));
}

template <class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) {
  T* p = dynamic_cast<typename shared_ptr<T>::value_type*>(ptr.get());
  if (nullptr == p)
    return shared_ptr<T>();
  return shared_ptr<T>(ptr, p);
}
}  // namespace fstl

#pragma once

#include "../algorithm.h"
#include "../debug.h"
#include "../iterator.h"
#include "../memory.h"

#define INIT_SIZE 16
namespace fstl {

template <class T, class Allocator = fstl::allocator<T> >
class vector {
  static_assert(!std::is_same<bool, T>::value,
                "vector<bool> is abandoned in fstl");
  // 嵌套型别定义
 public:
  typedef fstl::allocator<T> allocator_type;

  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef fstl::reverse_iterator<iterator> reverse_iterator;
  typedef fstl::reverse_iterator<const_iterator> const_reverse_iterator;

  allocator_type get_allocator() { return allocator_type(); }

 private:
  iterator begin_ = nullptr;
  iterator end_ = nullptr;
  iterator cap_ = nullptr;
  // 构造、复制、移动、析构函数
 public:
  vector() noexcept { fill_init(0, value_type()); }
  explicit vector(size_type n) { fill_init(n, value_type()); }
  vector(size_type n, const value_type& value) { fill_init(n, value); }

  template <class Iter,
            typename std::enable_if<fstl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  vector(Iter first, Iter last) {
    range_init(first, last);
  }
  vector(const vector& rhs) { range_init(rhs.being(), rhs.end()); }
  vector(vector&& rhs) noexcept
      : begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_) {
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
  }
  vector(std::initializer_list<value_type> ilist) {
    range_move(ilist.begin(), ilist.end());
  }
  vector& operator=(const vector& rhs) {
    if (&rhs == this)
      return *this;
    ~vector();
    range_init(rhs.begin(), rhs.end());
    return *this;
  }
  vector& operator=(vector&& rhs) noexcept {
    if (&rhs == this)
      return *this;
    ~vector();
    begin_ = rhs.begin_;
    end_ = rhs.end_;
    cap_ = rhs.cap_;
    return *this;
  }
  vector& operator=(std::initializer_list<value_type> ilist) {
    ~vector();
    *this = vector(ilist);
    return *this;
  }
  ~vector() {
    get_allocator().destroy(begin_, cap_);
    get_allocator().deallocate(begin_);
  }
  // 迭代器相关操作
 public:
  iterator begin() noexcept { return begin_; }
  const_iterator begin() const noexcept { return begin_; }
  iterator end() noexcept { return end_; }
  const_iterator end() const noexcept { return end_; }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend() const noexcept { return end(); }
  const_reverse_iterator crbegin() const noexcept { return rbegin(); }
  const_reverse_iterator crend() const noexcept { return rend(); }
  // 容量相关操作
 public:
  //容量相关操作
  bool empty() const noexcept { return begin_ == end_; }

  size_type size() const noexcept {
    return static_cast<size_type>(end_ - begin_);
  }

  size_type max_size() const noexcept {
    return static_cast<size_type>(-1) / sizeof(T);
  }

  size_type capacity() const noexcept {
    return static_cast<size_type>(cap_ - begin_);
  }

  void reserve(size_type n) {
    size_type now = size();
    if (n <= now)
      return;
    add_mem(n - now);
    end_ = end_ - (n - now);
  }

  void shrink_to_fit() {
    get_allocator().deallocate(end_, cap_);
    cap_ = end_;
  }

  // 访问元素相关操作
 public:
  reference operator[](size_type n) {
    FSTL_DEBUG(n < size());
    return *(begin_ + n);
  }

  const_reference operator[](size_type n) const {
    FSTL_DEBUG(n < size());
    return *(begin_ + n);
  }

  reference at(size_type n) {
    THROW_LENGTH_ERROR_IF(!(n < size()),
                          "vector<T>::at() subscript out of range");
    return (*this)[n];
  }
  const_reference at(size_type n) const {
    THROW_OUT_OF_RANGE_IF(!(n < size()),
                          "vector<T>::at() subscript out of range");
    return (*this)[n];
  }

  reference front() {
    FSTL_DEBUG(!empty());
    return *begin_;
  }

  const_reference front() const {
    FSTL_DEBUG(!empty());
    return *begin_;
  }

  reference back() {
    FSTL_DEBUG(!empty());
    return *(end_ - 1);
  }

  const_reference back() const {
    FSTL_DEBUG(!empty());
    return *(end_ - 1);
  }

  pointer data() noexcept { return begin_; }
  const_pointer data() const noexcept { return begin_; }

  // 修改容器相关操作
 public:
  void assign(size_type n, const value_type& value) {
    vector tmp(n, value);
    swap(tmp);
  }

  template <class Iter,
            typename std::enable_if<fstl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  void assgin(Iter first, Iter last) {
    vector tmp(first, last);
    swap(tmp);
  }

  void assgin(std::initializer_list<value_type> il) {
    vector tmp(il);
    swap(tmp);
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    FSTL_DEBUG(pos >= begin() && pos <= end());
    add_mem(1);
    uninitialized_move(pos, end_ - 1, pos + 1);
    get_allocator().construct(&(*pos), fstl::forward<Args>(args)...);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    add_mem(1);
    get_allocator().construct(&(*(end_ - 1)), fstl::forward<Args>(args)...);
  }

  void push_back(const value_type& value) {
    add_mem(1);
    get_allocator().construct(&(*(end_ - 1)), value);
  }
  void push_back(value_type&& value) { emplace_back(fstl::move(value)); }

  void pop_back() { reduce_mem(1); }

  iterator insert(const_iterator pos, const value_type& value) {
    insert(pos, 1, value);
  }
  iterator insert(const_iterator pos, value_type&& value) {
    emplace(pos, std::move(value));
  }
  iterator insert(const_iterator pos, size_type n, const value_type& value) {
    FSTL_DEBUG(pos >= begin() && pos <= end());
    add_mem(n);
    uninitialized_move(pos, end_ - n, pos + n);
    uninitialized_fill_n(pos, n, value);
  }

  template <class Iter,
            typename std::enable_if<fstl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  void insert(const_iterator pos, Iter first, Iter last) {
    FSTL_DEBUG(pos >= begin() && pos <= end());
    size_type n = fstl::distance(first, last);
    add_mem(n);
    uninitialized_move(pos, end_ - n, pos + n);
    uninitialized_copy(first, last, pos);
  }

  iterator earse(const_iterator pos) {
    FSTL_DEBUG(pos >= begin() && pos <= end());
    earse(pos, pos + 1);
  }
  iterator earse(const_iterator first, const_iterator last) {
    FSTL_DEBUG(first >= begin() && first <= end());
    FSTL_DEBUG(last >= begin() && last <= end());
    size_type n = fstl::distance(first, last);
    uninitialized_copy(last, end_, first);
    reduce_mem(n);
  }

  void clear() { reduce_mem(size()); }

  void resize(size_type new_size) { resize(new_size, value_type()); }
  void resize(size_type new_size, const value_type& value) {
    if (new_size > size()) {
      add_mem(new_size - size());
      fill_init(new_size - size(), value);
    } else if (new_size < size()) {
      reduce_mem(size() - new_size);
    }
  }

  void swap(vector& rhs) noexcept {
    if (this != rhs) {
      fstl::swap(begin_, rhs.begin_);
      fstl::swap(end_, rhs.end_);
      fstl::swap(cap_, rhs.cap);
    }
  }

 private:
  void add_mem(size_type addSize) {
    FSTL_DEBUG(addSize >= 0);
    size_type curSize = fstl::distance(begin_, end_);
    size_type curCapSize = fstl::distance(begin_, cap_);

    if (curSize + addSize > curCapSize) {
      size_type newCapSize = fstl::max((int)(curSize + addSize) * 2, INIT_SIZE);

      iterator newBegin = get_allocator().allocate(newCapSize);
      uninitialized_move(begin_, end_, newBegin);

      begin_ = newBegin;
      cap_ = newBegin + newCapSize;
    }
    end_ = begin_ + curSize + addSize;
  }

  void reduce_mem(size_type removeSize) {
    FSTL_DEBUG(removeSize >= 0);
    FSTL_DEBUG(size() >= removeSize);

    size_type curSize = fstl::distance(begin_, end_);
    size_type curCapSize = fstl::distance(begin_, cap_);

    iterator newEnd = curSize - removeSize;
    get_allocator().destroy(newEnd, end_);
    end_ = newEnd;

    if (newEnd < curCapSize / 2 && curCapSize / 2 > INIT_SIZE) {
      size_type newCapSize = curCapSize / 2;
      iterator newCap = cap_ - newCapSize;
      get_allocator().deallocate(newCap, cap_);
      cap_ = newCap;
    }
  }

  template <class Iter>
  void range_move(Iter first, Iter last) {
    size_type size = fstl::distance(first, last);
    add_mem(size);
    uninitialized_move(first, last, end_ - size);
  }

  template <class Iter>
  void range_init(Iter first, Iter last) {
    size_type size = fstl::distance(first, last);
    add_mem(size);
    uninitialized_copy(first, last, end_ - size);
  }
  void fill_init(size_type size, value_type value) {
    add_mem(size);
    uninitialized_fill_n(end_ - size, size, value);
  }
};

/*****************************************************************************************/
// 重载比较操作符

// template <class T> bool operator==(const vector<T> &lhs, const vector<T>
// &rhs) {
//   return lhs.size() == rhs.size() &&
//          fstl::equal(lhs.begin(), lhs.end(), rhs.begin());
// }

// template <class T> bool operator<(const vector<T> &lhs, const vector<T> &rhs)
// {
//   return fstl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
//                                         lhs.end());
// }

// template <class T> bool operator!=(const vector<T> &lhs, const vector<T>
// &rhs) {
//   return !(lhs == rhs);
// }

// template <class T> bool operator>(const vector<T> &lhs, const vector<T> &rhs)
// {
//   return rhs < lhs;
// }

// template <class T> bool operator<=(const vector<T> &lhs, const vector<T>
// &rhs) {
//   return !(rhs < lhs);
// }

// template <class T> bool operator>=(const vector<T> &lhs, const vector<T>
// &rhs) {
//   return !(lhs < rhs);
// }

// // 重载 fstl 的 swap
// template <class T> void swap(vector<T> &lhs, vector<T> &rhs) { lhs.swap(rhs);
// }

};  // namespace fstl

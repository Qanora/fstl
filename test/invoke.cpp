
#include <../include/functional/invoke.h>

#include <iostream>

struct Foo {
  Foo(int num) : num_(num) {}
  void print_add(int i) const { std::cout << num_ + i << '\n'; }
  int num_;
};

void print_num(int i) {
  std::cout << i << '\n';
}

struct PrintNum {
  void operator()(int i) const { std::cout << i << '\n'; }
};

int main() {
  // invoke a free function
  fstl::invoke(print_num, -9);

  // invoke a lambda
  fstl::invoke([]() { print_num(42); });

  // invoke a member function
  const Foo foo(314159);
  fstl::invoke(&Foo::print_add, foo, 1);

  // invoke (access) a data member
  // auto c = std::invoke(&Foo::num_, foo);
  std::cout << "num_: " << fstl::invoke(&Foo::num_, foo) << '\n';

  // invoke a function object
  fstl::invoke(PrintNum(), 18);
}
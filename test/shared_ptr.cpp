//
// Created by Jinze Li on 2020/4/9.
//
#include <iostream>

#include "memory.h"
struct A {
  int a;
};
struct B : A {
  int b;
};
int main() {
  fstl::shared_ptr<B> pb;
  pb.reset(new B{2});
  auto c = fstl::weak_ptr(pb);
  std::cout << "test" << std::endl;
  pb.reset(nullptr);
  fstl::shared_ptr<B> f = c.lock();
  std::cout << "test" << std::endl;
  long d = c.use_count();
  std::cout << "test " << d << std::endl;
}

#include <iostream>

#include "../include/functional.h"

void test(int a, int b) {
  std::cout << a << b << std::endl;
}
int main(int argc, char* argv[]) {
  int z = 1;
  auto c = fstl::bind(test, fstl::placeholder::_1, z);
  c(2);
}
#include <iostream>

#include "../include/functional.h"
void test(int a) {
  std::cout << a << std::endl;
}
int main(int argc, char* argv[]) {
  auto c = fstl::bind(test, 1);
  c();
}
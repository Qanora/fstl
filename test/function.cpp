#include <iostream>

#include "../include/functional.h"

int main() {
  int i = 0;
  auto fn = [i](std::string const& s) mutable {
    std::cout << ++i << ". " << s << std::endl;
  };
  fn("first");   // 1. first
  fn("second");  // 2. second

  // construct from lambda
  fstl::function<void(std::string const&)> f(fn);
  f("third");  // 3. third

  // copy from another function
  fstl::function<void(std::string const&)> g(f);
  f("forth - f");  // 4. forth - f
  g("forth - g");  // 4. forth - g

  // capture and copy non-trivial types like std::string
  std::string x("xxxx");
  fstl::function<void()> h([x]() { std::cout << x << std::endl; });
  h();

  fstl::function<void()> k(h);
  k();
  return 0;
}
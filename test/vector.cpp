#include "../include/container/vector.h"

#include <iostream>

using namespace fstl;
int main(int argc, char* argv[]) {
  vector<int> a(10, 0);
  for (int i = 0; i < 10; i++) {
    a[i] = i;
    std::cout << a[i] << " " << std::endl;
  }
}
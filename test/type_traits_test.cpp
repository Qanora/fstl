//
// Created by Jinze Li on 2020/4/7.
//
#include "../include/type_traits.h"

#include <iostream>

template <size_t... Seq>
void testmakeindexSeq(fstl::index_sequence<Seq...>) {
  using unused = int[];
  (void)unused{0, (std::cout << Seq << ",", 0)...};
  std::cout << std::endl;
}

template <class T, T v>
void test() {
  testmakeindexSeq(fstl::make_index_sequence<v>());
}
int main() {
  test<size_t, 5>();
}
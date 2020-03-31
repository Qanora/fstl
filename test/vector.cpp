#include <iostream>
#include "container/vector.h"

using namespace fstl;
int main(int argc, char *argv[]) {
	vector<int> a(10, 0);
	for(int i = 0; i < 10; i++){
		std::cout << a[i] << " ";
	}
}
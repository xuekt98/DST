#include "..\header\Vector\Vector.h"

#include <cstdlib>
#include <iostream>
#include <ctime>

template <typename T>
struct increase {
	void operator()(T& a) {
		std::cout << a << std::endl; 
	}
};

int main() {
	using std::cout;
	using std::endl;
	DST::Vector<int> v;
	for(int i = 0; i < 50; ++i) {
		v.insert(rand() % 50);
	}

	clock_t start = clock(), finish;
	v.tranverse(increase<int>());
	finish = clock();
	cout << finish - start << endl;
	system("pause");
}
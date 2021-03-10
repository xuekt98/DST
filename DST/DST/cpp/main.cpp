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

void testDSTVector() {
	DST::Vector<int> v;
	for(int i = 0; i < 50; ++i) {
		v.insert(rand() % 50);
	}
}

int main() {
	using std::cout;
	using std::endl;

	clock_t start, finish;
	start = clock();
	finish = clock();
	cout << finish - start << endl;
	system("pause");
}
#ifndef DST_H
#define DST_H

#define NULL nullptr
#define max(a, b) (((a) > (b)) ? (a) : (b));

namespace DST {
	typedef int Rank;
	const static int DEFAULT_CAPACITY = 3;

	template <typename T>
	void swap(T& a, T& b) {				//��������Ԫ��
		T temp = a;
		a = b;
		b = temp;
	}

	template <typename T>
	bool lt(T* a, T* b) {				//С��(ָ��)
		return lt(*a, *b);
	}

	template <typename T>
	bool lt(T& a, T& b) {				//С��(����)
		return a < b;
	}

	template <typename T>
	bool eq(T* a, T* b) {				//����(ָ��)
		return eq(*a, *b);
	}

	template <typename T>
	bool eq(T& a, T& b) {				//����(����)
		return a == b;
	}

	template <typename T>
	Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {		//���ֲ���
		while(lo < hi) {
			Rank mi = (lo + hi) >> 1;
			(e < A[mi]) ? hi = mi : lo = mi + 1;
		}
		return --lo;
	}

	template <typename T>
	Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {		//���ֲ���
		while(lo < hi) {
			Rank mi = (hi - lo) * 0.618 + lo;
			(e < A[mi]) ? hi = mi : lo = mi + 1;
		}
		return --lo;
	}
}

#endif
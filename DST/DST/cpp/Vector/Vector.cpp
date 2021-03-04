#include "..\..\header\Vector\Vector.h"
#include <cstdlib>

using DST::Vector;
using DST::Rank;

/******�����ӿ�******/
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {	//���临��
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while(lo < hi) {
		_elem[_size++] = A[lo++];
	}
}

template <typename T>
void Vector<T>::expand() {									//�����ռ䲻��ʱ����
	if(_size < _capacity) return;
	if(_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];
	for(int i = 0; i < _size; ++ i) {
		_elem[i] = oldElem[i];
	}
	delete[] oldElem;
}

template <typename T>
void Vector<T>::shrink() {									//װ�����ӹ�Сʱѹ������
	if(_capacity < DEFAULT_CAPACITY << 1) return;			//����������DEFAULT_CAPACITY����
	if(_size << 2 > _capacity) return;						//װ����������25%
	T* oldELem = _elem;
	_elem = new T[_capacity >>= 1];
	for(int i = 0; i < _size; ++ i) {
		_elem[i] = oldElem[i];
	}
	delete oldElem;
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {					//һ��ð�ݵĹ���
	bool sorted = true;
	while(++lo < hi) {
		if(_elem[lo - 1] > _elem[lo]) {
			sorted = false;
			swap(_elem[lo - 1], _elem[lo]);
		}
	}
	return sorted;
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {				//ð������
	while(!bubble(lo, hi--));
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank hi, Rank mi) {			//�鲢
	T* A = _elem + lo;
	int lb = mi - lo;
	T* B = new T[lb];
	for(Rank i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi;
	T* C = _elem + mi;
	for(Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
		if((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if((k < lc) && (!(j < lb) || (C[k] < B[j]))) A[i++] = C[k++];
	}
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {			//�鲢����
	if(hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

/******ֻ�����ʽӿ�******/
//��������˳�����,�������һ��Ԫ��e��λ��,ʧ���򷵻�lo - 1
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while((lo < hi--) && (e != _elem[hi]));
	return hi;
}

//�ж�����,������������Ե�����
template <typename T>
int Vector<T>::disordered() const {
	int n = 0;
	for(int i = 1; i < _size; ++ i) {
		if(_elem[i - 1] > _elem[i]) {
			++n;
		}
	}
	return n;
}

//����������������[lo, hi)��,ȷ��������e�����һ���ڵ����
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
	return binSearch(_elem, e, lo, hi);
	//return fibSearch(_elem, e, lo, hi);
}

/******��д���ʽӿ�******/
template <typename T>
Vector<T>& Vector<T>::operator= (Vector<T> const& V) {		//���ظ�ֵ������
	if(_elem) delete[] _elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}

template <typename T>
T& Vector<T>::operator[] (Rank r) const {					//ѭ�ȷ���
	return _elem[r];
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {					//����ɾ��[lo, hi)
	if(lo == hi) return 0;
	while(hi < _size) {
		_elem[lo++] = _elem[hi++];
	}
	_size = lo;
	shrink();
	return hi - lo;
}

template <typename T>
T Vector<T>::remove(Rank r) {								//����Ԫ��ɾ��
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {							//����Ԫ��
	expand();	//�����ǰ�Ĺ�ģ�Ѿ����������,����Ҫ������
	for(int i = _size; i > r; --i) {
		_elem[i] = _elem[i - 1];
	}
	return r;
}

template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {					//�ȸ��������������
	T* V = _elem + lo;
	for(Rank i = hi - lo; i > 0; -- i) {
		swap(V[i - 1], V[rand() % i]);
	}
}

template <typename T>
int Vector<T>::deduplicate() {								//��������ȥ��
	int oldSize = _size;
	Rank i = 1;
	while(i < _size) {
		(find(_elem, 0, i) < 0) ? i++ : remove(i);
	}
	return oldSize - _size;
}

template <typename T>
int Vector<T>::uniquify() {									//��������ȥ��
	Rank i = 0, j = 0;
	while(++j < _size) {
		if(_elem[i] != _elem[j]) {
			_elem[++i] = _elem[j];
		}
	}
	_size = ++i;
	shrink();
	return j - i;
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
	bubbleSort(lo, hi);
}

/******����******/
template <typename T>
void Vector<T>::tranverse(void(*visit)(T&)) {				//ʹ�ú���ָ��
	for(int i = 0; i < _size; ++i) {
		visit(_elem[i]);
	}
}

template <typename T>
template <typename VST>
void Vector<T>::tranverse(VST& visit) {						//ʹ�ú�������
	for(int i = 0; i < _size; ++i) {
		visit(_elem[i]);
	}
}


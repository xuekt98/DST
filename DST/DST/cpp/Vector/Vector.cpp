#include "..\..\header\Vector\Vector.h"
#include <cstdlib>

using DST::Vector;
using DST::Rank;

/******保护接口******/
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {	//区间复制
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while(lo < hi) {
		_elem[_size++] = A[lo++];
	}
}

template <typename T>
void Vector<T>::expand() {									//向量空间不足时扩容
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
void Vector<T>::shrink() {									//装填因子过小时压缩容量
	if(_capacity < DEFAULT_CAPACITY << 1) return;			//不致收缩到DEFAULT_CAPACITY以下
	if(_size << 2 > _capacity) return;						//装填因子至少25%
	T* oldELem = _elem;
	_elem = new T[_capacity >>= 1];
	for(int i = 0; i < _size; ++ i) {
		_elem[i] = oldElem[i];
	}
	delete oldElem;
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {					//一次冒泡的过程
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
void Vector<T>::bubbleSort(Rank lo, Rank hi) {				//冒泡排序
	while(!bubble(lo, hi--));
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank hi, Rank mi) {			//归并
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
void Vector<T>::mergeSort(Rank lo, Rank hi) {			//归并排序
	if(hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi);
}

/******只读访问接口******/
//无序向量顺序查找,返回最后一个元素e的位置,失败则返回lo - 1
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while((lo < hi--) && (e != _elem[hi]));
	return hi;
}

//判定乱序,返回相邻逆序对的总数
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

//在有序向量的区间[lo, hi)内,确定不大于e的最后一个节点的秩
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
	return binSearch(_elem, e, lo, hi);
	//return fibSearch(_elem, e, lo, hi);
}

/******可写访问接口******/
template <typename T>
Vector<T>& Vector<T>::operator= (Vector<T> const& V) {		//重载赋值操作符
	if(_elem) delete[] _elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}

template <typename T>
T& Vector<T>::operator[] (Rank r) const {					//循秩访问
	return _elem[r];
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {					//区间删除[lo, hi)
	if(lo == hi) return 0;
	while(hi < _size) {
		_elem[lo++] = _elem[hi++];
	}
	_size = lo;
	shrink();
	return hi - lo;
}

template <typename T>
T Vector<T>::remove(Rank r) {								//单个元素删除
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {							//插入元素
	expand();	//如果当前的规模已经和容量相等,则需要先扩容
	for(int i = _size; i > r; --i) {
		_elem[i] = _elem[i - 1];
	}
	return r;
}

template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {					//等概率随机置乱区间
	T* V = _elem + lo;
	for(Rank i = hi - lo; i > 0; -- i) {
		swap(V[i - 1], V[rand() % i]);
	}
}

template <typename T>
int Vector<T>::deduplicate() {								//无序向量去重
	int oldSize = _size;
	Rank i = 1;
	while(i < _size) {
		(find(_elem, 0, i) < 0) ? i++ : remove(i);
	}
	return oldSize - _size;
}

template <typename T>
int Vector<T>::uniquify() {									//有序向量去重
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

/******遍历******/
template <typename T>
void Vector<T>::tranverse(void(*visit)(T&)) {				//使用函数指针
	for(int i = 0; i < _size; ++i) {
		visit(_elem[i]);
	}
}

template <typename T>
template <typename VST>
void Vector<T>::tranverse(VST& visit) {						//使用函数对象
	for(int i = 0; i < _size; ++i) {
		visit(_elem[i]);
	}
}


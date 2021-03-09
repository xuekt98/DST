#ifndef DST_VECTOR_H
#define DST_VECTOR_H

#include "..\DST.h"

namespace DST {
	template <typename T>
	class Vector {
	protected:
		Rank _size;		//��ģ
		int _capacity;	//����
		T* _elem;		//������

		void copyFrom(T const* A, Rank lo, Rank hi);	//������������A[lo, hi)
		void expand();	//�ռ䲻��ʱ����
		void shrink();	//װ�����ӹ�Сʱѹ��
		
		bool bubble(Rank lo, Rank hi);		//ɨ�轻��
		void bubbleSort(Rank lo, Rank hi);	//��������

		//Rank max(Rank lo, Rank hi);				//ѡȡ���Ԫ��
		//void selectionSort(Rank lo, Rank hi);	//ѡ�������㷨

		void merge(Rank lo, Rank mi, Rank hi);	//�鲢�㷨
		void mergeSort(Rank lo, Rank hi);		//�鲢�����㷨

		//Rank partition(Rank lo, Rank hi);		//��㹹���㷨
		//void quickSort(Rank lo, Rank hi);		//���������㷨
		
		//void heapSort(Rank lo, Rank hi);		//������

	public:
	//���캯��
		Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {		//����c����ģs������Ԫ�س�ʼΪv
			_elem = new T[_capacity = c];
			for(_size = 0; _size < s; _elem[_size++] = v); 
		}

		Vector(T const* A, Rank n) {				//�������帴��
			copyFrom(A, 0, n);
		}

		Vector(T const* A, Rank lo, Rank hi) {		//���临��
			copyFrom(A, lo, hi);
		}

		Vector(Vector<T> const& V) {				//�������캯��
			copyFrom(V._elem, 0, V._size());
		}

		Vector(Vector<T> const& V, Rank lo, Rank hi) {	//���俽��
			copyFrom(V._elem, lo, hi);
		}

	//��������
		~Vector() { delete[] _elem; }	//��������

	//ֻ�����ʽӿ�
		Rank size() const { return _size; }		//��ģ
		bool empty() const { return !_size; }	//�п�
		int disordered() const;					//�ж��Ƿ�������

		Rank find(T const& e) const { return find(e, 0, _size); }	//���������������
		Rank find(T const& e, Rank lo, Rank hi) const;				//���������������

		Rank search(T const& e) const {								//���������������
			return (0 >= _size) ? -1 : search(e, 0, _size);
		}
		Rank search(T const& e, Rank lo, Rank hi) const;					//���������������

	//��д���ʽӿ�
		T& operator[] (Rank r) const;					//�����±������
		Vector<T> & operator= (Vector<T> const&);		//���ظ�ֵ������

		T remove(Rank r);								//ɾ����Ϊr��Ԫ��
		int remove(Rank lo, Rank hi);					//����ɾ��

		Rank insert(T const& e) {						//Ĭ����ΪĩԪ�ز���
			return insert(_size, e);
		}
		Rank insert(Rank r, T const& e);				//����Ԫ��

		void sort(Rank lo, Rank hi);					//��[lo, hi)����
		void sort() { sort(0, _size); }					//��������

		void unsort(Rank lo, Rank hi);					//��[lo, hi)����
		void unsort() { unsort(0, _size); }				//��������

		int deduplicate();								//��������ȥ��
		int uniquify();									//��������ȥ��
	
	//����
		void tranverse(void(*) (T&));					//������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
		template <typename VST> void tranverse(VST&);	//������ʹ�ú�������ȫ�����޸ģ�
	};

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
}

#endif //~DST_VECTOR_H
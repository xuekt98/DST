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
}

#include "..\..\cpp\Vector\Vector.cpp"
#endif //~DST_VECTOR_H
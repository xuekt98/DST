#ifndef DST_LIST_H
#define DST_LIST_H

#include "ListNode.h"
#include "..\DST.h"

namespace DST {
	template <typename T>
	class List {
	private:
		int _size;					//��ģ
		ListNodePosi(T) header;		//ͷ�ڱ�
		ListNodePosi(T) trailer;	//β�ڱ�

	protected:
	//�����ӿ�
		void init();	//�б���ʱ�ĳ�ʼ��
		int clear();	//������нڵ�
		void copyNodes(ListNodePosi(T), int);	//�����б���λ��p���n��

		void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);	//�鲢
		void mergeSort(ListNodePosi(T)&, int);								//�鲢����

		void selectionSort(ListNodePosi(T), int);	//ѡ������
		void insertionSort(ListNodePosi(T), int);	//��������

	public:
	//���캯��
		List() { init(); }							//Ĭ�Ϲ��캯��
		List(List<T> const& L);						//���帴���б�L
		List(List<T> const& L, Rank r, int n);		//�����б�L�д�r�ʼ��n��
		List(ListNodePosi(T) p, int n);				//�����б��λ��p���n��

	//��������
		~List();

	//ֻ�����ʽӿ�
		Rank size() const { return _size; }			//��ģ
		Rank empty() const { return _size <= 0; }	//�п�
		T& operator[] (Rank r) const;				//����[],ѭ�ȷ���
		ListNodePosi(T) first() const { return header->succ; }		//�׽ڵ�λ��
		ListNodePosi(T) last() const { return trailer->pred; }		//ĩ�ڵ�λ��

		bool valid(ListNodePosi(T) p) {				//�ж�λ��p�Ƿ����Ϸ�
			return p && (trailer != p) && (header != p);
		}
		int disordered() const;						//�ж��Ƿ�������

		ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;	//�����������
		ListNodePosi(T) find(T const& e) const {							//�����б����
			return find(e, _size, trailer);
		}

		ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;	//�����������
		ListNodePosi(T) search(T const& e) const {							//�����б����
			return search(e, _size, trailer);
		}

		ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);				//p����n-1�����ѡ�������
		ListNodePosi(T) selectMax() {										//���������
			return selectMax(header->succ, _size);
		}
	
	//��д���ʽӿ�
		ListNodePosi(T) insertAsFirst(T const& e);					//��e��Ϊ�׽ڵ����
		ListNodePosi(T) insertAsLast(T const& e);					//��e��Ϊĩ�ڵ����
		ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e);		//��e����p�ĺ�̲���
		ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e);		//��e����p��ǰ������
		
		T remove(ListNodePosi(T) p);			//ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���Ľڵ�
		void merge(List<T>& L) {				//ȫ�б�鲢
			merge(first(), size, L, L.first(), L._size);
		}
		
		void sort(ListNodePosi(T) p, int n);	//�б���������
		void sort() {							//�б���������
			sort(first(), _size);
		}
		
		int deduplicate();		//�����б�ȥ��
		int uniquify();			//�����б�ȥ��
		void reverse();			//ǰ����

	//����
		void tranverse(void(*)(T&));	//����(����ָ��)
		template <typename VST>
		void tranverse(VST&);			//����(��������)
	};

	/******���캯��******/
	template <typename T>
	List<T>::List(ListNodePosi(T) p, int n) {	//�����б�����λ��p���n��
		copyNodes(p, n);
	}

	template <typename T>
	List<T>::List(List<T> const& L) {			//���帴���б�L
		copyNodes(L.first(), L._size);
	}

	template <typename T>
	List<T>::List(List<T> const& L, int r, int n) {		//����L���Ե�r�����n��
		copyNodes(L[r], n);
	}

	/******��������******/
	template <typename T>
	List<T>::~List() {
		clear();
		delete header;
		delete trailer;
	}

	/******�����ӿ�******/
	template <typename T>
	void List<T>::init() {				//�б��ʼ��,�ڴ����б�ʱͳһ����
		header = new ListNode<T>;
		trailer = new ListNode<T>;
		header->succ = trailer;
		header->pred = NULL;
		trailer->succ = header;
		trailer->pred = NULL;
		_size = 0;
	}

	template <typename T>
	void List<T>::copyNodes(ListNodePosi(T) p, int n) {		//�б��ڲ�����,�����б�����λ��p���n��
		init();
		while(n--) {
			insertAsLast(p->data);
			p = p->succ;
		}
	}
	
	template <typename T>
	int List<T>::clear() {			//����б�
		int oldSize = _size;
		while(0 < _size) {
			remove(header->succ);
		}
		return oldSize;
	}

	template <typename T>
	void List<T>::insertionSort(ListNodePosi(T) p, int n) {		//��������:����ʼ��p��n��Ԫ������
		for(int r = 0; r < n; ++r) {
			insertA(search(p->data, r, p), p->data);
			p = p->succ;
			return (p->pred);
		}
	}

	template <typename T>
	void List<T>::selectionSort(ListNodePosi(T) p, int n) {		//ѡ������
		ListNodePosi(T) head = p->pred;
		ListNodePosi(T) tail = p;
		for(int i = 0; i < n; ++i) {
			tail = tail->succ;
		}
		while(1 < n) {
			ListNodePosi(T) max = selectMax(head->succ, n);
			insertB(tail, remove(max));
			tail = tail->pred;
			n--;
		}
	}

	template <typename T>
	void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
		if(n < 2) return;
		int m = n >> 1;
		ListNodePosi(T) q = p;
		for(int i = 0; i < m; i++) q = q->succ;
		mergeSort(p, m);
		mergeSort(q, n - m);
		merge(p, m, *this, q, n - m);
	}

	template <typename T>
	void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
		ListNodePosi(T) pp = p->pred;
		while(0 < m) {
			if((0 < n) && (p->data <= q->data)) {
				if(q == (p = p->succ)) break;
				n--;
			}
			else {
				insertB(p, L.remove((q = q->succ)->pred));
				m--;
			}
		}
		p = pp->succ;
	}

	/******ֻ�����ʽӿ�******/
	template <typename T>
	T& List<T>::operator[] (Rank r) const {		//ѭ�ȷ���
		ListNodePosi(T) p = first();
		while(0 < r--) p = p->succ;
		return p->data;
	}

	//�������б��ڽڵ�p(������trailer)��n��(��)ǰ����,�ҵ�����e�������
	template <typename T>
	ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
		while(0 < n--) {
			if(e == (p = p->pred)->data) return p;
		}
		return NULL;
	}

	//�������б��ڽڵ�p(������trailer)��n��(��)ǰ����,�ҵ�������e�������
	template <typename T>
	ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
		while(0 <= n--) {
			if(((p = p->pred)->data) <= e) break;
		}
		return p;
	}

	template <typename T>
	ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) {		//����ʼ��p��n��Ԫ�����ҳ������
		ListNodePosi(T) max = p;
		for(ListNodePosi(T) cur = p; 1 < n; n--) {
			if(!lt((cur = cur->succ)->data, max->data)) 
				max = cur;
		}
		return max;
	}

	/******��д���ʽӿ�******/
	template <typename T>
	ListNodePosi(T) List<T>::insertAsFirst(T const& e) {		//�����׽ڵ����
		++_size;
		return header->insertAsSuccc(e);
	}

	template <typename T>
	ListNodePosi(T) List<T>::insertAsLast(T const& e) {			//����ĩ�ڵ����
		++_size;
		return trailer->insertAsPred(e);
	}

	template <typename T>
	ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e) {	//e����p�ĺ�̲���
		++_size;
		return p->insertAsSucc(e);
	}

	template <typename T>
	ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e) {	//e����p��ǰ������
		++_size;
		return p->insertAsPred(e);
	}

	template <typename T>
	T List<T>::remove(ListNodePosi(T) p) {		//ɾ���Ϸ��ڵ�p,��������ֵ
		T e = p->data;
		p->pred->succ = p->succ;
		p->succ->pred = p->pred;
		delete p;
		--_size;
		return e;
	}

	template <typename T>
	void List<T>::sort(ListNodePosi(T) p, int n) {		//�б���������
		
	}

	template <typename T>
	int List<T>::deduplicate() {		//�����б�ȥ��
		if(_size < 2) return 0;
		int oldSize = _size;
		ListNodePosi(T) p = header;
		Rank r = 0;
		while(trailer != (p = p->succ)) {	//p���׽ڵ㿪ʼ
			ListNodePosi(T) p = find(p->data, r, p);
			q ? remove(q) : r++;
		}
		return oldSize - _size;
	}

	template <typename T>
	int List<T>::uniquify() {			//�����б�ȥ��
		if(_size < 2) return 0;
		int oldSize = _size;
		ListNodePosi(T) p = first();
		ListNodePosi(T) q;
		while(trailer != (q = p->succ)) { //�����ͬ,ɾ������
			if(p->data != q->data) p = q;
			else remove(q);
		}
		return oldSize - _size;
	}

	/******����******/
	template <typename T>
	void List<T>::tranverse(void (*visit) (T&)) {
		for(ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) {
			visit(p->data);
		}
	}

	template <typename T>
	template <typename VST>
	void List<T>::tranverse(VST& visit) {
		for(ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) {
			visit(p->data);
		}
	}
}

#endif //~DST_LIST_H
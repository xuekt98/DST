#ifndef DST_BINTREE_H
#define DST_BINTREE_H

#include "BinNode.h"
#include <memory>

namespace DST {
	template <typename T>
	class BinTree {
	protected:
		int _size;
		BinNodePosi(T) _root;
		virtual int updateHeight(BinNodePosi(T) x);
		void updateHeightAbove(BinNodePosi(T) x);

	public:
		BinTree() : _size(0), _root(nullptr) {}		//���캯��
		~BinTree() {								//��������
			if(_size > 0) remove(_root);
		}
		int size() const { return _size; }				//��ģ
		bool empty() const { return !_root; }			//�п�
		BinNodePosi(T) root() const { return _root; }	//����

		BinNodePosi(T) insertAsRoot(T const& e);		//������ڵ�
		BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);		//e��Ϊx�����Ӳ���
		BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);		//e��Ϊx���Һ��Ӳ���
		BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T);	//T��Ϊx������������
		BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T);	//T��Ϊx������������
		int remove(BinNodePosi(T) x);			//ɾ����λ��x���ڵ�Ϊ��������,���ظ�����ԭ�ȵĹ�ģ
		BinTree<T>* secede(BinNodePosi(T) x);	//������x�ӵ�ǰ����ժ��,��ת����һ�ö�������

		template <typename VST>
		void travLevel(VST& visit) {			//��α���
			if(_root) _root->travLevel(visit);
		}

		template <typename VST>
		void travPre(VST& visit) {				//�������
			if(_root) _root->travPre(visit);
		}

		template <typename VST>
		void travIn(VST& visit) {				//�������
			if(_root) _root->travIn(visit);
		}

		template <typename VST>
		void travPost(VST& visit) {				//�������
			if(_root) _root->travPost(visit);
		}

		bool operator< (BinTree<T> const& t) {				//С��
			return _root && t._root && lt(_root, t._root);
		}

		bool operator== (BinTree<T> const& t) {				//����
			return _root && t._root && (_root == t._root);
		}
	};

	template <typename T>
	int BinTree<T>::updateHeight(BinNodePosi(T) x) {	//���½ڵ�x�ĸ߶�
		return x->height = 1 + max(stature(x->lc), stature(x->rc));
	}

	template <typename T>
	void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {	//���¸߶�
		while(x) {
			updateHeight(x);
			x = x->parent;
		}
	}

	template <typename T>
	BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) {	//��e�������ڵ����յĶ�����
		_size = 1;
		return _root = new BinNode<T>(e);
	}

	template <typename T>
	BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) {	//e����Ϊx������
		_size++;
		x->insertAsLC(e);
		updateHeightAbove(x);
		return x->lc;
	}

	template <typename T>
	BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e) {	//e����Ϊx������
		_size++;
		x->insertAsRC(e);
		updateHeightAbove(x);
		return x->rc;
	}

	template <typename T>	//�����������㷨:��S��Ϊ�ڵ�x������������,S�����ÿ�
	BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) {
		if(x->lc = S->_root) x->lc->parent = x;
		_size += S->_size;
		updateHeightAbove(x);
		S->_root = NULL;
		S->_size = 0;
		//release(S);
		S = NULL;
		return x;
	}

	template <typename T>	//�����������㷨:��S��Ϊ�ڵ�x������������,S�����ÿ�
	BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) {
		if(x->rc = S->_root) x->rc->parent = x;
		_size += S->_size;
		updateHeightAbove(x);
		S->_root = nullptr;
		S->_size = 0;
		//release(S);
		S = nullptr;
		return x;
	}

	template <typename T>	//ɾ����������λ��x���Ľڵ㼰����,���ر�ɾ���ڵ����ֵ
	int BinTree<T>::remove(BinNodePosi(T) x) {
		FromParentTo(*x) = NULL;
		updateHeightAbove(x->parent);
		int n = removeAt(x);
		_size = n;
		return n;
	}

	template <typename T>	//���������������㷨:������x�ӵ�ǰ����ժ��,�����װ��һ�ö�������������
	BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
		FromParentTo(*x) = NULL;
		updateHeightAbove(x->parent);
		BinTree<T>* S = new BinTree<T>;
		S->_root = x;
		x->parent = nullptr;
		S->_size = x->size();
		_size -= S->_size;
		return S;
	}


	/******���������ڲ��ӿ�******/
	template <typename T>	//ɾ����������λ��x���Ľڵ㼰����,���ر�ɾ���ڵ����ֵ
	static int removeAt(BinNodePosi(T) x) {
		if(!x) return 0;
		int n = 1 + removeAt(x->lc) + removeAt(x->rc);
		release(x->data);
		release(x);
		return n;
	}
}

#endif //~DST_BINTREE_H
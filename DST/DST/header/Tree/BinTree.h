#ifndef DST_BINTREE_H
#define DST_BINTREE_H

#include "BinNode.h"

namespace DST {
	template <typename T>
	class BinTree {
	protected:
		int _size;
		BinNodePosi(T) _root;
		virtual int updateHeight(BinNodePosi(T) x);
		void updateHeightAbove(BinNodePosi(T) x);

	public:
		BinTree() : _size(0), _root(NULL) {}		//���캯��
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
}

#endif //~DST_BINTREE_H
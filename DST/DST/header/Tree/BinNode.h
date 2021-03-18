#ifndef DST_BINNODE_H
#define DST_BINNODE_H

#include "..\Stack\Stack.h"
#include "..\Queue\Queue.h"

#define BinNodePosi(T) BinNode<T>*				//�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1)		//�ڵ�߶�
typedef enum{RB_RED, RB_BLACK} RBColor;			//�������ɫ

#define IsRoot(x) (!((x).parent))								//�Ƿ��Ǹ��ڵ�
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))	//�Ƿ�������
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))	//�Ƿ�������
#define HasParent(x) (!IsRoot(x))								//�Ƿ��и��ڵ�
#define HasLChild(x) ((x).lc)									//�Ƿ�������
#define HasRChild(x) ((x).rc)									//�Ƿ�������
#define HasChild(x) (HasLChild(x) || HasRChild(x))				//�Ƿ����ӽڵ�
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))			//�Ƿ��������ӽڵ�
#define IsLeaf(x) (!HasChild(x))								//�Ƿ���Ҷ�ڵ�

#define sibling(p)/*�ֵܽڵ�*/ \
	(IsChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)
#define uncle(x)/*����ڵ�*/ \
	(IsChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc)
#define FromParentTo(x)/*���Ը��ڵ������*/ \
	(IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))

namespace DST {
	//�������ڵ�ģ����
	template <typename T>
	struct BinNode {
	//��Ա
		T data;
		BinNodePosi(T) parent;	//���ڵ�
		BinNodePosi(T) lc;		//����
		BinNodePosi(T) rc;		//�Һ���
		int height;				//�߶�
		int npl;				//Null Path Length(��ʽ��)
		RBColor color;			//��ɫ(�����)

	//���캯��
		BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
		BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
				int h = 0, int l = 1, RBColor c = RB_RED) : 
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}

	//�����ӿ�
		int size();		//ͳ�Ƶ�ǰ�ڵ�������
		BinNodePosi(T) insertAsLC(T const&);	//��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
		BinNodePosi(T) insertAsRC(T const&);	//��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
		BinNodePosi(T) succ();					//ȡ�õ�ǰ�ڵ��ֱ�Ӻ��
		template <typename VST> void travLevel(VST&);		//������α���
		template <typename VST> void travPre(VST&);			//�����������
		template <typename VST> void travIn(VST&);			//�����������
		template <typename VST> void travPost(VST&);		//�����������

	//�Ƚ���,�е���
		bool operator< (BinNode const& bn) { return data < bn.data; }		//С��
		bool operator== (BinNode const& bn) { return data == bn.data; }		//����
	};

	template  <typename T>
	BinNodePosi(T) BinNode<T>::insertAsLC(T const&) {		//��Ϊ���Ӳ����½ڵ�
		return lc = new BinNode(e, this);
	}

	template <typename T>
	BinNodePosi(T) BinNode<T>::insertAsRC(T const&) {		//��Ϊ�Һ��Ӳ����½ڵ�
		return rc = new BinNode(e, this);
	}

	template <typename T>
	BinNodePosi(T) BinNode<T>::succ() {						//��λ�ڵ�v��ֱ�Ӻ��
		BinNodePosi(T) s = this;
		if(rc) {
			s = rc;
			while(HasLChild(*s)) s = s->lc;
		}
		else {
			while(IsRChild(*s)) s = s->parent;
			s = s->parent;
		}
		return s;
	}

	template <typename T>
	template <typename VST>
	void BinNode<T>::travIn(VST& visit) {					//�������
		
	}

	template <typename T>
	template <typename VST>
	void BinNode<T>::travLevel(VST& visit) {				//�������
		Queue<BinNodePosi(T)> Q;
		Q.enqueue(this);
		while(!Q.empty()) {
			BinNodePosi(T) x = Q.dequeue();
			visit(x->data);
			if(HasLChild(*x)) Q.enqueue(x->lc);
			if(HasRChild(*x)) Q.enqueue(x->rc);
		}
	}

	/***�ݹ������㷨***/
	template <typename T, typename VST>		//�ݹ���������
	void travPre_R(BinNodePosi(T) x, VST& visit) {
		if(!x) return;
		visit(x->data);
		travPre_R(x->lc, visit);
		travPre_R(x->rc, visit);
	}

	template <typename T, typename VST>		//�ݹ��������
	void travPost_R(BinNodePosi(T) x, VST& visit) {
		if(!x) return;
		travPost_R(x->lc, visit);
		travPost_R(x->rc, visit);
		visit(x->data);
	}

	template <typename T, typename VST>		//�ݹ���������
	void travIn_R(BinNodePosi(T) x, VST& visit) {
		if(!x) return;
		travIn_R(x->lc, visit);
		visit(x->data);
		travIn_R(x->rc, visit);
	}

	/***����������㷨***/
	/**�������������**/
	template <typename T, typename VST>
	static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S) {
		while(x) {
			visit(x->data);
			S.push(x->rc);
			x = x->lc;
		}
	}

	template <typename T, typename VST>		//�������������
	void travPre_I2(BinNodePosi(T) x, VST& visit) {
		Stack<BinNodePosi(T)> S;
		while(true) {
			visitAlongLeftBranch(x, visit, S);
			if(S.empty()) break;
			x = S.pop();
		}
	}

	/**�������������**/
	template <typename T>
	static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S) {
		while(x) {
			S.push(x);
			x = x->lc;
		}
	}

	template <typename T, typename VST>
	void travIn_I1(BinNodePosi(T) x, VST& visit) {
		Stack<BinNodePosi(T)> S;
		while(true) {
			goAlongLeftBranch(x, S);
			if(S.empty()) break;
			x = S.pop();
			visit(x->data);
			x = x->rc;
		}
	}

	template <typename T, typename VST>
	void travIn_I2(BinNodePosi(T) x, VST& visit) {
		Stack<BinNodePosi(T)> S;
		while(true) {
			if(x) {
				S.push(x);
				x = x->lc;
			}
			else if(!S.empty()) {
				x = S.pop();
				visit(x->data);
				x = x->rc;
			}
			else {
				break;
			}
		}
	}

	template <typename T, typename VST>
	void travIn_I3(BinNodePosi(T) x, VST& visit) {
		bool backtrack = false;
		while(true) {
			if(!backtrack && HasLChild(*x)) x = x->lc;
			else {
				visit(x->data);
				if(HasRChild(*x)) {
					x = x->rc;
					backtrack = false;
				}
				else {
					if(!(x = x->succ())) break;
					backtrack = true;
				}
			}
		}
	}

	/**������������**/
	template <typename T>
	static void gotoHLVFL(Stack<BinNodePosi(T)>& S) {
		while(BinNodePosi(T) x = S.top()) {
			if(HasLChild(*x)) {
				if(HasRChild(*x)) S.push(x->rc);
				S.push(x->lc);
			}
			else {
				S.push(x->rc);
			}
		}
	}

	template <typename T, typename VST>
	void travPost_I(BinNodePosi(T) x, VST& visit) {
		Stack<BinNodePosi(T)> S;
		if(x) S.push(x);
		while(!S.empty()) {
			if(S.top() != x->parent)
				gotoHLVFL(S);
			x = S.pop();
			visit(x->data); 
		}
	}
}

#endif //~DST_BINNODE_H
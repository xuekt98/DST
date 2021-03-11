#ifndef DST_BINNODE_H
#define DST_BINNODE_H

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
	(IsRoot(x) ? _root : (IsChild(x) ? (x).parent->lc : (x).parent->rc))

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
	void BinNode<T>::travIn(VST& visit) {					//�������
		
	}
}

#endif //~DST_BINNODE_H
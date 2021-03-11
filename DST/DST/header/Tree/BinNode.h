#ifndef DST_BINNODE_H
#define DST_BINNODE_H

#define BinNodePosi(T) BinNode<T>*				//节点位置
#define stature(p) ((p) ? (p)->height : -1)		//节点高度
typedef enum{RB_RED, RB_BLACK} RBColor;			//红黑树颜色

#define IsRoot(x) (!((x).parent))								//是否是根节点
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))	//是否是左子
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))	//是否是右子
#define HasParent(x) (!IsRoot(x))								//是否有父节点
#define HasLChild(x) ((x).lc)									//是否有左子
#define HasRChild(x) ((x).rc)									//是否有右子
#define HasChild(x) (HasLChild(x) || HasRChild(x))				//是否有子节点
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))			//是否有两个子节点
#define IsLeaf(x) (!HasChild(x))								//是否是叶节点

#define sibling(p)/*兄弟节点*/ \
	(IsChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)
#define uncle(x)/*叔叔节点*/ \
	(IsChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc)
#define FromParentTo(x)/*来自父节点的引用*/ \
	(IsRoot(x) ? _root : (IsChild(x) ? (x).parent->lc : (x).parent->rc))

namespace DST {
	//二叉树节点模板类
	template <typename T>
	struct BinNode {
	//成员
		T data;
		BinNodePosi(T) parent;	//父节点
		BinNodePosi(T) lc;		//左孩子
		BinNodePosi(T) rc;		//右孩子
		int height;				//高度
		int npl;				//Null Path Length(左式堆)
		RBColor color;			//颜色(红黑树)

	//构造函数
		BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
		BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
				int h = 0, int l = 1, RBColor c = RB_RED) : 
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}

	//操作接口
		int size();		//统计当前节点后代总数
		BinNodePosi(T) insertAsLC(T const&);	//作为当前节点的左孩子插入新节点
		BinNodePosi(T) insertAsRC(T const&);	//作为当前节点的右孩子插入新节点
		BinNodePosi(T) succ();					//取得当前节点的直接后继
		template <typename VST> void travLevel(VST&);		//子树层次遍历
		template <typename VST> void travPre(VST&);			//子树先序遍历
		template <typename VST> void travIn(VST&);			//子树中序遍历
		template <typename VST> void travPost(VST&);		//子树后序遍历

	//比较器,判等器
		bool operator< (BinNode const& bn) { return data < bn.data; }		//小于
		bool operator== (BinNode const& bn) { return data == bn.data; }		//等于
	};

	template  <typename T>
	BinNodePosi(T) BinNode<T>::insertAsLC(T const&) {		//作为左孩子插入新节点
		return lc = new BinNode(e, this);
	}

	template <typename T>
	BinNodePosi(T) BinNode<T>::insertAsRC(T const&) {		//作为右孩子插入新节点
		return rc = new BinNode(e, this);
	}

	template <typename T>
	void BinNode<T>::travIn(VST& visit) {					//中序遍历
		
	}
}

#endif //~DST_BINNODE_H
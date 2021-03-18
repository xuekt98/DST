#ifndef DST_GRAPH_H
#define DST_GRAPH_H

#include "..\Stack\Stack.h"
#include "..\Queue\Queue.h"
#include <climits>

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;	//����״̬
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;	//���ڱ�����������������

namespace DST {
	template <typename Tv, typename Te>		//��������,������
	class Graph {
	private:
		void reset() {	//���ж���,�ߵĸ�����Ϣ��λ
			for(int i = 0; i < n; ++i) {
				status(i) = UNDISCOVERED;
				dTime(i) = fTime(i) = -1;
				parent(i) = -1;
				priority(i) = INT_MAX;
				for(int j = 0; j < n; ++j) {
					if(exists(i, j)) type(i, j) = UNDETERMINED;
				}
			}
		}

		void BFS(int, int&);	//������������㷨
		void DFS(int, int&);	//������������㷨
		void BCC(int, int&, Stack<int>&);	//����DFS��˫��ͨ�����ֽ��㷨
		bool TSort(int, int&, Stack<Tv>*);	//����DFS�����������㷨
		template <typename PU> void PFS(int, PU);	//���ȼ��������

	public:
		int n;	//��������
		virtual int insert(Tv const&) = 0;		//���붥��,���ر��
		virtual Tv remove(int) = 0;				//ɾ�����㼰�������,���ظö�����Ϣ
		virtual Tv& vertex(int) = 0;			//����v������
		virtual int inDegree(int) = 0;			//�������
		virtual int outDegree(int) = 0;			//�������
		virtual int firstNbr(int, int) = 0;		//�׸��ڽӶ���
		virtual int nextNbr(int, int) = 0;		//��һ���ڽӶ���
		virtual VStatus& status(int) = 0;		//����״̬
		virtual int& dTime(int) = 0;			//dTime
		virtual int& fTime(int) = 0;			//fTime
		virtual int& parent(int) = 0;			//�������еĸ���
		virtual int& priority(int) = 0;			//�������е����ȼ�

		int e;	//������
		virtual bool exists(int, int) = 0;		//��(u, v)�Ƿ����
		virtual void insert(Te const&, int, int, int) = 0;	//����(u, v)Ȩ��w�ı�
		virtual Te remove(int, int) = 0;		//ɾ��u��v֮��ı�e,���رߵ���Ϣ
		virtual EType& type(int, int) = 0;		//��(u, v)������
		virtual Te& edge(int, int) = 0;			//��(u, v)������
		virtual int& weight(int, int) = 0;		//��(u, v)��Ȩ��

		void bfs(int);			//�����������
		void dfs(int);			//�����������
		void bcc(int);			//����DFS��˫��ͨ�����ֽ��㷨
		Stack<Tv>* tSort(int);	//����DFS�����������㷨
		void prim(int);			//��С֧�����㷨
		void dijkstra(int);		//���·���㷨
		template <typename PU> void pfs(int, PU);	//���ȼ��������
	};

	//�����������
	template <typename Tv, typename Te>
	void Graph<Tv, Te>::bfs(int s) {
		reset();
		int clock = 0;
		int v = s;
		do {
			if(status(v) == UNDISCOVERED) BFS(v, clock);
		} while(s != (v = (++v % n)));
	}

	template <typename Tv, typename Te>
	void Graph<Tv, Te>::BFS(int v, int& clock) {
		Queue<int> Q;
		status(v) = DISCOVERED;
		Q.enqueue(v);
		while(!Q.empty()) {
			int v = Q.dequeue();
			dTime(v) = ++clock;
			for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
				if(UNDISCOVERED == status(u)) {
					status(u) = DISCOVERED;
					Q.enqueue(u);
					type(v, u) = TREE;
					parent(u) = v;
				}
				else {
					type(v, u) = CROSS;
				}
			}
			status(v) = VISITED;
		}
	}

	//�����������
	template <typename Tv, typename Te>
	void Graph<Tv, Te>::dfs(int s) {
		reset();
		int clock = 0;
		int v = s;
		do {
			if(status(v) == UNDISCOVERED) DFS(v, clock);
		} while(s != (v = (++v % n)));
	}

	template <typename Tv, typename Te>
	void Graph<Tv, Te>::DFS(int v, int& clock) {
		dTime(v) = ++clock;
		status(v) = DISCOVERED;
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
			switch(status(u)) {
			case UNDISCOVERED:
				type(v, u) = TREE;
				parent(u) = v;
				DFS(u, clock);
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD;
				break;
			default:
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		}
		status(v) = VISITED;
		fTime(v) = ++clock;
	}

	//��������
	template <typename Tv, typename Te>
	Stack<Tv>* Graph<Tv, Te>::tSort(int s) {
		reset();
		int clock = 0;
		int v = s;
		Stack<Tv>* S = new Stack<Tv>;
		do {
			if(UNDISCOVERED == status(v)) {
				if(!TSort(v, clock, S)) {
					while(!S->empty())
						S->pop();
					break;
				}
			}
		} while(s != (v = (++v % n)));
		return S;
	}

	template <typename Tv, typename Te>
	bool Graph<Tv, Te>::TSort(int v, int& clock, Stack<Tv>* S) {
		dTime(v) = ++clock;
		status(v) = DISCOVERED;
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
			switch(status(u)) {
			case UNDISCOVERED:
				parent(u) = v;
				type(v, u) = TREE;
				if(!TSort(u, clock, S)) return false;
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD;
				return false;
			default:
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		}
		status(v) = VISITED;
		S->push(vertex(v));
		return true;
	}

	//˫��ͨ��ֽ�
	template <typename Tv, typename Te>
	void Graph<Tv, Te>::bcc(int s) {
		reset();
		int clock = 0, v = s;
		Stack<int> S;
		do {
			if(UNDISCOVERED == status(v)) {
				BCC(v, clock, S);
				S.pop();
			}
		} while(s != (v = (++v % n)));
	}

	#define hac(x) (fTime(x))

	template <typename Tv, typename Te>
	void Graph<Tv, Te>::BCC(int v, int& clock, Stack<int>& S) {
		hca(v) = dTime(v) = ++clock;
		status(v) = DISCOVERED;
		S.push(x);
		for(int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
			switch(status(u)) {
			case UNDISCOVERED:
				parent(u) = v;
				type(v, u) = TREE;
				BCC(u, clock, S);
				if(hca(u) < dTime(v))
					hac(v) = min(hca(v), hca(u));
				else {
					while(v != S.pop());
					S.push(v);
				}
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD;
				if(u != parent(v))
					hca(v) = min(hca(v), dTime(v));
				break;
			default:
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		}
		status(v) = VISITED;
	}

	//���ȼ�����
	template<typename Tv, typename Te>
	template<typename PU>
	void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {
		reset();
		int v = s;
		do {
			if(UNDISCOVERED == status(v))
				PFS(v, prioUpdater);
		} while(s != (v = (++v % n)));
	}

	template <typename Tv, typename Te>
	template <typename PU>
	void Graph<Tv, Te>::PFS(int s, PU prioUpdater) {
		priority(s) = 0;
		status(s) = VISITED;
		parent(s) = -1;
		while(1) {
			for(int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) {
				prioUpdater(this, s, w);
			}
			for(int shortest = INT_MAX, w = 0; w < n; ++w) {
				if(UNDISCOVERED == status(v)) {
					if(shortest > priority(w)) {
						shortest = priority(w);
						s = w;
					}
				}
			}
			if(VISITED == status(s)) break;
			status(s) = VISITED;
			type(parent(s), s) = TREE;
		}
	}

	//���prim�㷨�Ķ������ȼ�������
	template <typename Tv, typename Te>
	struct PrimPU {
		virtual void operator() (Graph<Tv, Te>* g, int uk, int v) {
			if(UNDISCOVERED == g->status(v))
				if(g->priority(v) > g->weight(uk, v)) {
					g->priority(v) = g->weight(uk, v);
					g->parent(v) = uk;
				}
		}
	};

	//���Dijkstra�㷨�Ķ������ȼ�������
	template <typename Tv, typename Te>
	struct  DijkstraPU {
		virtual void operator() (Graph<Tv, Te>* g, int uk, int v) {
			if(UNDISCOVERED == g->status(v))
				if(g->priority(v) > g->priority(uk) + g->weight(uk, v)) {
					g->priority(v) = g->priority(uk) + g->weight(uk, v);
					g->parent(v) = uk;
				}
		}
	};
}

#endif //~DST_GRAPH_H
#ifndef FIBHEAP_H
#define FIBHEAP_H

typedef struct Node {
	struct Node *parent;
	struct Node *left;
	struct Node *right;
	struct Node *child;
	int key;
	int degree;
	int value;
	int mark;
} Node;

typedef struct {
	Node *min;
	int count;
} Heap;

Heap *createHeap();
Heap *fibHeapInsert(Heap */*heap*/, int /*key*/, int /*value*/);
void fibHeapAddNodeToRootList(Node */*node*/, Heap */*heap*/);
Node *fibHeapMin(Heap */*heap*/);
void print(Heap */*heap*/);
Heap *mergeHeaps(Heap */*heap1*/, Heap */*heap2*/);
void fibHeapRemoveNodeFromRootList(Node */*node*/);
Node *fibHeapDeleteMin(Heap */*heap*/);
void fibheapUnionList(Node* /*node*/, Node*  /*node*/);
void FibHeapConsolidate(Heap */*heap*/);
void fibheapLink(Node* /*node*/, Node* /*child*/);
int fibHeapMaxDegree(Heap* /*heap*/);
void fibheapDecreaseKey(Heap* /*heap*/, Node* /*node*/, int /*key*/);
void fibheapCut(Heap* /*heap*/, Node* node_c, Node* /*node*/);
void fibheapCascadingCut(Heap* /*heap*/, Node* /*node*/);


#endif
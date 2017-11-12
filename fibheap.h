#ifndef FIBHEAP_H
#define FIBHEAP_H

typedef struct node {
	struct node *parent;
	struct node *left;
	struct node *right;
	struct node *child;
	int key;
	int degree;
	int value;
	int mark;
} node;

typedef struct {
	node *min;
	int count;
} heap;

heap *createHeap();
heap *fibHeapInsert(heap */*heap*/, int /*key*/, int /*value*/);
void fibHeapAddNodeToRootList(node */*node*/, heap */*heap*/);
node *fibHeapMin(heap */*heap*/);
void print(heap */*heap*/);
heap *mergeHeaps(heap */*heap1*/, heap */*heap2*/);

#endif
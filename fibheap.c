#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "fibheap.h"

heap *createHeap()
{
	heap *hp = malloc(sizeof(heap));

	hp->min = NULL;
	hp->count = 0;

	return hp;	
}

heap *fibHeapInsert(heap *hp, int key, int value)
{
	node *N = malloc(sizeof(node));
	N->parent = NULL;
	N->left = N;
	N->right = N;
	N->child = NULL;
	N->key = key;
	N->degree = 0;
	N->value = value;
	N->mark = 0;

	fibHeapAddNodeToRootList(N, hp);

	if (hp->min == NULL || N->key < hp->min->key) {
		hp->min = N;
	}
	hp->count++;

	return hp;
}

void fibHeapAddNodeToRootList(node *N, heap *hp)
{
	if (hp->min == NULL) {
		return;
	}

	if (hp->min->left == hp->min) {
		hp->min->left = N;
		hp->min->right = N;
		N->right = hp->min;
		N->left = hp->min;
	} else {
		N->right = hp->min;
		N->left = hp->min->left;
		hp->min->left->right = N;
		hp->min->left = N;
	}
}

node *fibHeapMin(heap *hp)
{
	return hp->min;
}

void print(heap *hp)
{
	node *N = hp->min->left;
	printf("%d\n", hp->min->key);
	while (N != hp->min) {
		printf("%d\n", N->key);
		N = N->left;
	}
}

heap *mergeHeaps(heap *hp1, heap *hp2)
{
	if (!hp1) {
		hp1 = createHeap();
	} else if (!hp2) {
		hp2 = createHeap();
	}

	heap *hp = createHeap();

	if (hp1->min && hp2->min) {
		hp->min = hp1->min;
		hp->min->left->right = hp2->min->right;
		hp2->min->right->left = hp->min->left;
		hp->min->left = hp2->min;
		hp2->min->right = hp->min;
		if ((hp1->min == NULL) || ((hp2->min) && (hp2->min->key <= hp1->min->key))) {
			hp->min = hp2->min;
		}
	} else {
		if (hp1->min) {
			hp->min = hp1->min;
		} else if (hp2->min) {
			hp->min = hp2->min;
		}
	}
	
	hp->count = hp1->count + hp2->count;
	free(hp1);
	free(hp2);
	return hp;
}

node *fibHeapDeleteMin(heap *hp)
{
	node *min = hp->min;
	if (min == NULL) {
		return NULL;
	}

	node *tmp = min->child;
	while (tmp) {
		fibHeapAddNodeToRootList(tmp, hp);
		tmp->parent = NULL;
		if (tmp->left == tmp) {
			break;
		}
		tmp = tmp->left;
	}
	fibHeapRemoveNodeFromRootList(min);

	if (min == min->right) {
		hp->min = NULL;
	} else {
		hp->min = min->right;
		FibHeapConsolidate(hp);
	}
	
	hp->count--;

	return min;
}

void fibHeapRemoveNodeFromRootList(node *n)
{
	n->right->left = n->left;
	n->left->right = n->right;
}
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "fibheap.h"

Heap *createHeap()
{
	Heap *hp = malloc(sizeof(Heap));

	hp->min = NULL;
	hp->count = 0;

	return hp;	
}

Heap *fibHeapInsert(Heap *hp, int key, int value)
{
	Node *N = malloc(sizeof(Node));
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

void fibHeapAddNodeToRootList(Node *N, Heap *hp)
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

Node *fibHeapMin(Heap *hp)
{
	return hp->min;
}

void print(Heap *hp)
{
	Node *N = hp->min->left;
	printf("%d\n", hp->min->key);
	while (N != hp->min) {
		printf("%d\n", N->key);
		N = N->left;
	}
}

Heap *mergeHeaps(Heap *hp1, Heap *hp2)
{
	if (!hp1) {
		hp1 = createHeap();
	} else if (!hp2) {
		hp2 = createHeap();
	}

	Heap *hp = createHeap();

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

Node *fibHeapDeleteMin(Heap *hp)
{
	Node *min = hp->min;
	if (min == NULL) {
		return NULL;
	}

	Node *tmp = min->child;
	while (tmp) {
		fibHeapAddNodeToRootList(tmp, hp);
		tmp->parent = NULL;
		if (tmp->left == tmp) {
			break;
		}
		tmp = tmp->left;
	}

	fibheapUnionList(min, min->child);
	fibHeapRemoveNodeFromRootList(min);

	if (min == min->right) {
		hp->min = NULL;
	} else {
		hp->min = min->right;
		FibHeapConsolidate(hp);
	}

	hp->count--;
	free(min);

	return min;
}

void fibHeapRemoveNodeFromRootList(Node *n)
{
	n->right->left = n->left;
	n->left->right = n->right;
}

void fibheapUnionList(Node* min_a, Node* min_b)
{
	if (min_a == NULL || min_b == NULL)
		return;
	min_a->right->left = min_b->left;
	min_b->left->right = min_a->right;
	min_a->right = min_b;
	min_b->left = min_a;
}

Heap *FibHeapConsolidate(Heap *heap)
{
	int degree = fibHeapMaxDegree(heap), n = 0;
	Node** arr_Node = malloc(sizeof(Node) * degree);
	Node* node = NULL;

	for (int i = 0; i <= degree; i++) {
		arr_Node[i] = NULL;
	}

	while (node != heap->min) {
		if (node == NULL)
			node = heap->min;
		n++;
		node = node->right;
	}

	for (int i = 0; i < n; i++) {
		int n_degree = node->degree;
		while (arr_Node[n_degree] != NULL) {
			if (arr_Node[n_degree]->key < node->key) {
				if (arr_Node[n_degree]->child != NULL) {
					fibheapLink(arr_Node[n_degree], node);
				} else {
					arr_Node[n_degree]->child = node;
					fibHeapRemoveNodeFromRootList(node);
					node->left = node;
					node->right = node;
					arr_Node[n_degree]->degree++;
					node->parent = arr_Node[n_degree];
				}
				node = arr_Node[n_degree];
			} else {
				if (node->child != NULL) {
					fibheapLink(node, arr_Node[n_degree]);
				} else {
					node->child = arr_Node[n_degree];
					fibHeapRemoveNodeFromRootList(arr_Node[n_degree]);
					arr_Node[n_degree]->left = arr_Node[n_degree];
					arr_Node[n_degree]->right = arr_Node[n_degree];
					node->degree++;
					arr_Node[n_degree]->parent = node;
				}
			}
			arr_Node[n_degree] = NULL;
			n_degree++;
		}	
		arr_Node[n_degree] = node;
		node = node->right;
	}

	for (int i = 0; i <= degree; i++) {
		if (arr_Node[i] != NULL) {
			if (heap->min == NULL || heap->min->key > arr_Node[i]->key) {
				heap->min = arr_Node[i];
			}
		}
	}

	return heap;
}

void fibheapLink(Node* node, Node* child)
{
	node->degree++;
	fibHeapRemoveNodeFromRootList(child);
	child->parent = node;
	fibHeapAddNodeToRootList(child, node->child);
	child->mark = 0;
}

int fibHeapMaxDegree(Heap* heap)
{
	return floor(log(heap->count));
}

void fibheapDecreaseKey(Heap* heap, Node* node, int new_key)
{
	Node* parent = node->parent;
	if (node->key < new_key)
		return;
	node->key = new_key;
	if (parent != NULL && parent->key > node->key) {
		fibheapCut(heap, node, parent);
		fibheapCascadingCut(heap, parent);
	}
	if (heap->min == NULL || node->key < heap->min->key) {
		heap->min = node;
	}
}

void fibheapCut(Heap* heap, Node* node_c, Node* node_p)
{
	if (node_c == node_p->child && node_c != node_c->right) {
		node_p->child = node_c->right;
	} else {
		node_p->child = NULL;
	}

	node_c->left->right = node_c->right;
	node_c->right->left = node_c->left;

	node_c->right = heap->min;
	node_c->left = heap->min->left;
	heap->min->left->right = node_c;
	heap->min->left = node_c;

	node_c->parent = NULL;
	node_p->degree--;
	node_c->mark = 0;
}

void fibheapCascadingCut(Heap* heap, Node* node)
{
	if (node->parent == NULL)
		return;
	if (node->mark == 0) {
		node->mark = 1;
	} else {
		fibheapCut(heap, node, node->parent);
		fibheapCascadingCut(heap, node->parent);
	}
}

void fibheapDeleteNode(Heap* heap, Node* node)
{
	fibheapDecreaseKey(heap, node, -1);
	fibHeapDeleteMin(heap);
}
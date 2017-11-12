#include "fibheap.h"
#include <stdio.h>

int main()
{
	heap *root = createHeap();
	root = fibHeapInsert(root, 5, 5);
	root = fibHeapInsert(root, 2, 2);
	root = fibHeapInsert(root, 3, 3);
	root = fibHeapInsert(root, 6, 6);
	print(root);

	printf("\n");

	heap *root2 = createHeap();
	root2 = fibHeapInsert(root2, 1, 1);
	root2 = fibHeapInsert(root2, 4, 4);
	root2 = fibHeapInsert(root2, 7, 7);
	root2 = fibHeapInsert(root2, 0, 0);
	print(root2);

	printf("\n");

	heap *root3 = mergeHeaps(root, root2);
	print(root3);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int main(){

	HEAP * heap = new_heap(8);
	heap_push(heap,6);
	heap_push(heap,3);
	heap_push(heap,4);
	heap_push(heap,7);
	heap_push(heap,9);
	heap_push(heap,2);
	heap_push(heap,1);

	printHeap(heap);
	heapSort(heap);
	printHeap(heap);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include <papi.h>
#include <time.h>


HEAP *new_heap(int size){
	HEAP *heap;
	posix_memalign((void**)&heap,64,sizeof(HEAP));
	posix_memalign((void**)&heap->nums,64,sizeof(long)*size);
	if(heap != NULL && heap->nums !=NULL){
		heap->end = -1;

	}else{
		printf("No memory available for new heap\n");
	}
	return heap;
}

int empty_heap(HEAP *heap){
	return (heap->end == -1);
}

void swap(HEAP * heap, int actual, int new_pos){
	long tmp = heap->nums[actual];
	heap->nums[actual] = heap->nums[new_pos];
	heap->nums[new_pos] = tmp;
}
void perc_up(HEAP *heap){
	int pos = heap->end;
	int dad = (pos - 1)/2;
	while(pos > 0 && heap->nums[pos] > heap->nums[dad]){
		swap(heap,pos,dad);
		pos = dad;
		dad = (dad -1)/2;
	}
}

void perc_down(HEAP *heap){
	int pos = 0;
	int largestChild;
	while(pos <= heap->end/2){
		int leftChild  = 2 * pos + 1;
		int rightChild = 2 * pos + 2;
		if(rightChild <= heap->end && 
			heap->nums[leftChild] < heap->nums[rightChild]){
			largestChild = rightChild;
		}else{
			largestChild = leftChild;
		}

		if(heap->nums[pos] >= heap->nums[largestChild]){
			break;
		}

		swap(heap,pos,largestChild);
		pos = largestChild;
	}
}

void heap_push(HEAP *heap, long num){
	heap->end++;
	heap->nums[heap->end] = num;
	perc_up(heap);
}

long heap_pop(HEAP *heap){
	if(!empty_heap(heap)){
		long num = heap->nums[0];
		heap->nums[0] = heap->nums[heap->end];
		heap->end--;
		if(!empty_heap(heap))
			perc_down(heap);
		return num;
	}else{
		printf("Heap is empty\n");
	}

	return -1;
}

void printHeap(HEAP * heap){
	int i;
	for(i=0; i<= heap->end;i++){
		printf("%li ",heap->nums[i]);
	}
	printf("\n");
}

void heapSort(HEAP *heap){
	int tempSize = heap->end;

	while(!empty_heap(heap)){
		long tempNum = heap_pop(heap);
		heap->nums[heap->end + 1] = tempNum;
	}
	heap->end = tempSize;
}


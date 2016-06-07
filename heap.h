#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef struct HEAP{
 	long *nums;
 	int end;
}HEAP;

HEAP *new_heap(int);
int empty_heap(HEAP *);
long heap_pop(HEAP *);
void swap(HEAP *, int, int);
void perc_up(HEAP *);
void perc_down(HEAP *);
void heap_push(HEAP *heap, long);
void printHeap(HEAP *);
void heapSort(HEAP *);



#endif
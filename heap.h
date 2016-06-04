#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED

typedef struct HEAP{
 	int *nums;
 	int end;
}HEAP;

HEAP *new_heap(int);
int empty_heap(HEAP *);
void swap(HEAP *, int, int);
void perc_up(HEAP *);
void perc_down(HEAP *);
void heap_push(HEAP *heap, int);
void printHeap(HEAP *);
int heap_pop(HEAP *);
void heapSort(HEAP *);



#endif
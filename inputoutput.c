#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100000
#define MIN -100000
#include "inputoutput.h"


long* geraArrayAleatorio(int tamanho){//Sendo n ao tamanho do array q eh potencia de 2^n/sizeof(long)
	long *arrayAleatorio;
	
	posix_memalign((void**)&arrayAleatorio,64,tamanho*sizeof(long));
	int i;
	for(i=0;i<tamanho;i++){
		arrayAleatorio[i] = (rand()%(MAX-MIN)+MIN);
	}
    return arrayAleatorio;

}

void insereHeap(int tam,long * arrayAleatorio,HEAP *heap){
	int i;
	for(i=0;i<tam;i++){
		heap_push(heap,arrayAleatorio[i]);
	}
}

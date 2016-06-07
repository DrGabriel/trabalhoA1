#include <stdio.h>
#include <stdlib.h>


void bubbleSort(long *nums,int size){
	int i,j;
	long temp;
	for(i=0; i<size; i++){
		for(j=0;j<(size- i - 1);j++){
			if(nums[j]>nums[j+1]){
				//swap
				temp = nums[j];
				nums[j] = nums[j+1];
				nums[j+1] = temp;
			}
		}
	}
}



int main(){

	int tam = 6;
	long * nums ; 
	posix_memalign((void**)&nums,64,sizeof(long) *tam);
	nums[0] = 2;
	nums[1] = 7;
	nums[2] = 4;
	nums[3] = 1;
	nums[4] = 5;
	nums[5] = 3;

	bubbleSort(nums,tam);

	int i;
	for(i=0;i<tam;i++){
		printf("%li \n",nums[i]);
	}

	return 0;
}




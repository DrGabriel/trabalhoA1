#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <time.h>
#include <math.h>

#include "heap.h"
#include "inputoutput.h"

#define NUM_EVENTS 2
#define TAM 8
#define RANGE 1
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

int main(){

    long_long start_cycles, end_cycles, start_usec, end_usec;
    srand(time(NULL));
    /*Declaring and initializing the event set with the presets*/
    int Events[NUM_EVENTS] = {PAPI_FP_OPS,PAPI_TOT_CYC},
    /*declaring place holder for no of hardware counters*/
    num_hwcntrs = 0,
    retval;
    char errstring[PAPI_MAX_STR_LEN];
    long_long values[NUM_EVENTS];

   /***************************************************************************
   *  This part initializes the library and compares the version number of the*
   * header file, to the version of the library, if these don't match then it *
   * is likely that PAPI won't work correctly.If there is an error, retval    *
   * keeps track of the version number.                                       *
   ***************************************************************************/

   if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
   {
      fprintf(stderr, "Error: %d %s\n",retval, errstring);
      exit(1);
   }

    /**************************************************************************
    * PAPI_num_counters returns the number of hardware counters the platform *
    * has or a negative number if there is an error                          *
    **************************************************************************/
   if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK)
   {
      printf("There are no counters available. \n");
      exit(1);
   }

   printf("There are %d counters in this system\n",num_hwcntrs);


  int tamanho = (pow(2,TAM))/sizeof(long);
	HEAP * heap = new_heap(tamanho);
  long *arr = geraArrayAleatorio(tamanho);
  insereHeap(tamanho,arr,heap);


	printHeap(heap);
	/**************************************************************************
    	* PAPI_start_counters initializes the PAPI library (if necessary) and    *
    	* starts counting the events named in the events array. This function    *
   	* implicitly stops and initializes any counters running as a result of   *
    	* a previous call to PAPI_start_counters.                                *
    	**************************************************************************/

    printf("\nCounter Started: \n");
   if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
       ERROR_RETURN(retval);



   	/* Gets the starting time in clock cycles */
   	start_cycles = PAPI_get_real_cyc();

   	/* Gets the starting time in microseconds */
    	start_usec = PAPI_get_real_usec();

	   heapSort(heap);


	/**********************************************************************
    	* PAPI_read_counters reads the counter values into values array      *
    	**********************************************************************/
    	/* Gets the ending time in clock cycles */
    	end_cycles = PAPI_get_real_cyc();

    	/* Gets the ending time in microseconds */
   	end_usec = PAPI_get_real_usec();

   	if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
      		ERROR_RETURN(retval);

   	printf("Read successfully\n");
   	printf("The total FLOPS for heapsort of size %d are:\n ----- %lld ---- \n",TAM,values[0]);
   	printf("The total cycles used are: \n ----- %lld ---- \n", values[1] );
   	printf("+----------------------------------------------------------------+\n");
   	printf("Wall clock cycles: %lld\n", end_cycles - start_cycles);
   	printf("+----------------------------------------------------------------+\n");
   	printf(" | Wall clock time in microseconds: %lld |\n", (end_usec - start_usec));
   	printf("+----------------------------------------------------------------+\n");


    	/******************* PAPI_stop_counters **********************************/
   	if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
      		ERROR_RETURN(retval);

	printHeap(heap);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <papi.h>
#include "inputoutput.h"

#define NUM_EVENTS 3
#define RANGE 1
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

void bubbleSort(long *nums,long size){
	long i,j,temp;
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

long * copiaArrayAleatorio(long* arr2,long tamanho){
	long * temp,i;
	posix_memalign((void**)&temp,64,tamanho*sizeof(long));

	for(i=0;i<tamanho;i++)
		temp[i]=arr2[i];

	return temp;
}

int main(){
    int i,count,expoente=10;
    FILE *saida = fopen("saidaBublleSort.txt","a+");
    long_long start_usec, end_usec,totalCycles= 0,totalTime = 0,totalFlips=0,totalFlops=0;
    /*Declaring and initializing the event set with the presets*/
    int Events[NUM_EVENTS] = {PAPI_TOT_CYC,PAPI_FP_INS,PAPI_FP_OPS},
    /*declaring place holder for no of hardware counters*/
    num_hwcntrs = 0,
    retval;
    char errstring[PAPI_MAX_STR_LEN];
    long_long values[NUM_EVENTS];

    srand(time(NULL));

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

     for(expoente;expoente<=20;expoente++){
        totalCycles = 0;
        totalFlips  = 0;
        totalFlops  = 0;
        totalTime   = 0;

       long tamanho = (pow(2,expoente))/sizeof(long);
       long *arr = geraArrayAleatorio(tamanho);
       for(count=0;count<10;count++){
            long* arrcpy = copiaArrayAleatorio(arr,tamanho);
          /**************************************************************************
          * PAPI_start_counters initializes the PAPI library (if necessary) and    *
          * starts counting the events named in the events array. This function    *
          * implicitly stops and initializes any counters running as a result of   *
          * a previous call to PAPI_start_counters.                                *
          **************************************************************************/
          if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
             ERROR_RETURN(retval);

          /* Gets the starting time in microseconds */
            start_usec = PAPI_get_real_usec();

           bubbleSort(arrcpy,tamanho);//Evento que sera calculado


          /* Gets the ending time in microseconds */
          end_usec = PAPI_get_real_usec();

          /**********************************************************************
          * PAPI_read_counters reads the counter values into values array      *
          **********************************************************************/
          if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
                ERROR_RETURN(retval);

          totalCycles+=values[0];
          totalFlips +=values[1];
          totalFlops +=values[2];
          totalTime += (end_usec - start_usec);


            /******************* PAPI_stop_counters **********************************/
          if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
              ERROR_RETURN(retval);
        }
        totalCycles /= 10;
        totalFlips  /= 10;
        totalFlops  /= 10;
        totalTime   /= 10;
      free(arr);

      long memoria = sizeof(long) * tamanho;
      double tempo =  (double)totalTime/1000000;

      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"BUBBLESORT tamanho:%ld expoente: %d\n",tamanho,expoente);
      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"Total de FLOPS: ----- %lld ---- \n",totalFlops);
      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"Total de FLIPS: ----- %lld ---- \n", totalFlips );
      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"Total de ciclos: ----- %lld ---- \n", totalCycles );
      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"Tempo em segundos: ----- %lf ---- \n",tempo);
      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"Dados trafegados por segundo: %lf\n",(double)(memoria/tempo));
      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"Total de bytes: %ld\n",memoria);
      fprintf(saida,"+----------------------------------------------------------------+\n\n");
      fflush(saida);
    }
	return 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <time.h>

#define NUM_EVENTS 3
#define RANGE 1
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

double* create_matrix(long, double*);
void show_matrix(long, double*);
double* mult_matrix(long,double*,double*);

int main(){

    long_long start_cycles, end_cycles, start_usec, end_usec,totalFlops=0,totalFlips=0,totalTime=0,totalCycles=0;
    long DIM;
    int counter;
    FILE *saida = fopen("saidaMatrizMult.txt","a+");

    printf("Qual sera a dimensao da matrix nxn: ");
    scanf("%ld",&DIM);
    srand(time(NULL));
    /*Declaring and initializing the event set with th epresets*/
    int Events[NUM_EVENTS] = {PAPI_FP_OPS,PAPI_TOT_CYC,PAPI_FP_INS},
    /*declaring place holder for no of hardware counters*/
    num_hwcntrs = 0,
    retval;
    char errstring[PAPI_MAX_STR_LEN];
    long_long values[NUM_EVENTS];
    for(DIM;DIM<=1048576;DIM*=2){
      totalTime   = 0;
      totalFlips  = 0;
      totalFlops  = 0;
      totalCycles = 0;
      double* matrix1 = create_matrix(DIM,matrix1);
      double* matrix2 = create_matrix(DIM,matrix2);

      printf("\nCounter Started: \n");
      for(counter=0;counter<10;counter++){
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

       start_usec = PAPI_get_real_usec();

          /**************************************************************************
        * PAPI_start_counters initializes the PAPI library (if necessary) and    *
        * starts counting the events named in the events array. This function    *
        * implicitly stops and initializes any counters running as a result of   *
        * a previous call to PAPI_start_counters.                                *
        **************************************************************************/

       if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
           ERROR_RETURN(retval);

       /* Gets the starting time in clock cycles */
       start_cycles = PAPI_get_real_cyc();

       /* Gets the starting time in microseconds */
        start_usec = PAPI_get_real_usec();


       /* Your code goes here*/

       double* result = mult_matrix(DIM,matrix1,matrix2);

       /**********************************************************************
        * PAPI_read_counters reads the counter values into values array      *
        **********************************************************************/
        /* Gets the ending time in clock cycles */
        end_cycles = PAPI_get_real_cyc();

        /* Gets the ending time in microseconds */
       end_usec = PAPI_get_real_usec();

       if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
          ERROR_RETURN(retval);

        totalFlops += values[0];
        totalFlips += values[2];
        totalCycles += values[1];
        totalTime += (end_usec - start_usec);

        /******************* PAPI_stop_counters **********************************/
       if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
          ERROR_RETURN(retval);

        free(result);
      }

      totalTime /=10;
      totalFlops /=10;
      totalCycles /=10;
      totalFlips /=10;

      long memoria = sizeof(double) * DIM * DIM * 3;
      double tempo =  (double)totalTime/1000000;

      fprintf(saida,"+----------------------------------------------------------------+\n");
      fprintf(saida,"MATRIX DIM:%ld x %ld\n",DIM,DIM);
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
      fprintf(saida,"+----------------------------------------------------------------+\n\n");
      fflush(saida);

      free(matrix1);
      free(matrix2);
  }
  fclose(saida);
  return 0;

}

double* create_matrix(long dim, double* matrix){

    int aux = posix_memalign((void**)&matrix,64,dim * dim * sizeof(double));

    long i,j;
    for (i=0; i< dim; i++){
        for(j= 0; j< dim; j++){
            matrix[i*dim + j] = (-1+ 2*((double)rand()/(double)(RAND_MAX)))*RANGE ;
        }
    }

   return matrix;

}

void show_matrix(long dim, double* matrix){
    printf("\n");
    long i,j;
    for(i=0;i<dim;i++){
        for(j=0;j<dim;j++){
            printf("%lf ",(matrix[i*dim + j]));
        }
        printf("\n");
   }
}

double* mult_matrix(long dim,double* matrix1,double* matrix2){

    double *result;
    int aux = posix_memalign((void**)&result,64, dim * dim * sizeof(double));
    float sum;
    long i,j,k=0,actual_pos=0,pos=0;

    for(i=0;i<dim;i++){

        for(j=0;j<dim;j++){

            sum = 0.0;

            for(k=0;k<dim;k++){

                sum += matrix1[i*dim + k] * matrix2[k*dim + j];
            }

            result[i*dim + j] = sum;

        }
    }

    return result;


}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include <malloc.h>
#include "papi.h"
#include <time.h>
#include <math.h>

#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

double** alocarMatriz(int tamanho);

float det(int n , double** a)
{
  int i,j,k,l;
  double d = 0;
  double** m = alocarMatriz(n-1);

  if(n==2)
      {
         d = (a[0][0]*a[1][1])-(a[0][1]*a[1][0]);
         //printf("passei\n"); 
         for(i=0;i<n;i++)
            free(a[i]);
               free(a);
         
         return(d);
      }
  else
  {
	  
	 for (k=0;k<n;k++)
  	 { 
	   printf("%d\n",k);
	   m = alocarMatriz(n-1);
	   for (i=1;i<n;i++) {
		   //"trhow away" the line 0
            l = 0;
            for (j=0;j<n;j++) {
               if (j == k) // trhow away the column [k] 
                  continue;
             
               m[i-1][l] = a[i][j];
               l++;
            }
         }
         d += pow(-1.0,k) * a[0][k] * det(n-1,m);
         //when k is even a sum happens, when k is odd a subtraction happens
                
	  }
	 
     
   }
   for(i=0;i<n;i++)
       free(a[i]);
   free(a);
   
    return d;
    
}


double** alocarMatriz(int tamanho){ 
 
  int i,j; //Variáveis Auxiliares
  double** matriz;
  
  posix_memalign((void**)&matriz,64,sizeof(double*)*tamanho);
	for(i=0;i<tamanho;i++)
	    posix_memalign((void**)&matriz[i],64,sizeof(double)*tamanho);
  

 
  for (i = 0; i < tamanho; i++){ //Percorre as linhas do Vetor de Ponteiros
       for (j = 0; j < tamanho; j++){ //Percorre o Vetor de Inteiros atual.
            matriz[i][j] = 0; //Inicializa com 0.
       }
  }
return matriz; //Retorna o Ponteiro para a Matriz Alocada
}


int main()
{
	int i,j,r,k;
	float sum =0;
    int retval;//papi
    long long values[3];
    char errstring[PAPI_MAX_STR_LEN];
    int num_hwcntrs = 0;
    int tamanho;
    double** a,**copia; 
    
    long long cicAcm = 0;
    long long flopAcm = 0;
    long long insAcm = 0;
    long long tempoAcm =0;
    
   
    
    long_long tempoInicio, tempoFim, tempo;
      
    int Events[3] = {PAPI_FP_INS,PAPI_TOT_CYC,PAPI_FP_OPS};

    FILE* saida = fopen("saidaLuRoof","w");
     
    srand( (unsigned)time(NULL) );
 
    tamanho = 8;
    while(tamanho<32768){    
		tamanho*=2;
		
		for(r=0;r<10;r++){	
			    
	        a = alocarMatriz(tamanho);
	        //copia = alocarMatriz(tamanho);
	        
            //do{
               for(i=0;i<tamanho;i++)
                   for(j=0;j<tamanho;j++){
                       a[i][j] = ((float)rand()/(float)(RAND_MAX)* rand());
                       //copia[i][j] = a[i][j];
                    }   
            //}
            //while( det(tamanho,copia) != 0);
             
           if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
           {
               fprintf(stderr, "Error: %d %s\n",retval, errstring);
               exit(1);
           }

           if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK)
           {
               printf("There are no counters available. \n");
               exit(1);
           }

           //printf("There are %d counters in this system\n",num_hwcntrs);
		
           tempoInicio = PAPI_get_real_usec(); 
           if ( (retval = PAPI_start_counters(Events,3)) != PAPI_OK)
               ERROR_RETURN(retval);
               
            for(i=0;i<tamanho;i++){
               for(j=0;j<tamanho;j++){
                   if(j<=i) 
                      for(k=0;k<=j-1;k++)
                          a[j][i]-=a[j][k]*a[k][i]; 
            
                   else{
               
                   for(k=0;k<=i-1;k++)
                       a[j][i]-=a[j][k]*a[k][i];
                   a[j][i]/=a[i][i];
                
                   }
               }
            }
          
          
          tempoFim = PAPI_get_real_usec();
    
         if ( (retval=PAPI_read_counters(values, 3)) != PAPI_OK)
             ERROR_RETURN(retval);
 
	     tempo = tempoFim - tempoInicio;
        
         insAcm += values[0];
         cicAcm += values[1];
         flopAcm += values[2];
         tempoAcm += tempo;
         
         for(i=0;i<tamanho;i++)
            free(a[i]);
         free(a);
             
         if((retval = PAPI_stop_counters(values,3)) != PAPI_OK)
             ERROR_RETURN(retval);
     }
     
     insAcm/=10;
     cicAcm/=10;
     flopAcm/=10;
     tempoAcm/=10;
     
     
     fprintf(saida,"FLIPS: %lld\n",insAcm);
     fprintf(saida,"CICLOS: %lld\n",cicAcm);
     fprintf(saida,"FLOPS: %lld\n",flopAcm);
     fprintf(saida,"TEMPO: %lld\n",tempoAcm);
     fprintf(saida,"BYTES: %d\n", tamanho*tamanho*8);
     
     fprintf(saida,"\n");
     fflush(saida);
  }
	
	return 0;
}


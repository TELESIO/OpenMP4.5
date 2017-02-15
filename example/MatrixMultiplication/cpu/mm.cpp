/******************************************************************************
* FILE: omp_mm.c
* DESCRIPTION:  
*   OpenMp Example - Matrix Multiply - C Version
*   Demonstrates a matrix multiply using OpenMP. Threads share row iterations
*   according to a predefined chunk size.
* AUTHOR: Blaise Barney
* LAST REVISED: 06/28/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "Time.h"

#define NRA 20000                 /* number of rows in matrix A */
#define NCA 20000                 /* number of columns in matrix A */
#define NCB 20000                 /* number of columns in matrix B */

int main (int argc, char *argv[]) 
{
int	tid, nthreads, i, j, k, chunk;
double	**a= (double**)malloc(sizeof(double*)*NRA);           /* matrix A to be multiplied */
    for (j=0; j<NRA; j++)
	a[j] = (double*)malloc(sizeof(double)*NCA);

double	**b=(double**)malloc(sizeof(double*)*NCA);           /* matrix B to be multiplied */
    for (j=0; j<NCA; j++)
	b[j] = (double*)malloc(sizeof(double)*NCB);

double	**c=(double**)malloc(sizeof(double*)*NRA);           /* result matrix C */
    for (j=0; j<NRA; j++)
	c[j] = (double*)malloc(sizeof(double)*NCB);

chunk = 10;                    /* set loop iteration chunk size */
  tid = omp_get_thread_num();

  /*** Initialize matrices ***/
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i*j;
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;
struct Time * time= (struct Time *)malloc(sizeof(struct Time));
startTime(time);
/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {

 /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)    
    {
    for(j=0; j<NCB; j++)       
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }

  }   /*** End of parallel region ***/
endTime(time);
printf ("Done.\n");

}

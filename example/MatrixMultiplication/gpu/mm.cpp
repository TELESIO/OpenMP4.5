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

#define NRA 5000                 /* number of rows in matrix A */
#define NCA 5000                 /* number of columns in matrix A */
#define NCB 5000                  /* number of columns in matrix B */

int main (int argc, char *argv[]) 
{
int	tid, nthreads, i, j, k, chunk;
double	*a= (double*)malloc(sizeof(double)*NRA*NCA);           /* matrix A to be multiplied */
double	*b=(double*)malloc(sizeof(double)*NCA*NCB);           /* matrix B to be multiplied */
double	*c=(double*)malloc(sizeof(double)*NRA*NCB);           /* result matrix C */
double	*check=(double*)malloc(sizeof(double)*NRA*NCB);           /* result matrix C */

chunk = 10;                    /* set loop iteration chunk size */

  /*** Initialize matrices ***/
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i*NRA+j]= i+j;
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i*NCA+j]= i*j;
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++){
      c[i*NRA+j]= 0;
        check[i*NRA+j]= 0;
        }
 

// for (i=0; i<NRA; i++)
//    {
//    for(j=0; j<NCB; j++)
//      for (k=0; k<NCA; k++)
	
//        check[i*NRA+j] += a[i*NRA+j] * b[k*NCA+j];
	
//    }
int DimA=NRA*NCA;
int DimB=NCB*NCA;
int DimC=NRA*NCA;
struct Time * time= (struct Time *)malloc(sizeof(struct Time));
startTime(time);
/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp target map(to: a[0:DimA],b[0:DimB]) map(tofrom: c[0:DimC])
#pragma omp teams distribute parallel for simd
  for (i=0; i<NRA; i++)
    for(j=0; j<NCB; j++)
      for(k=0; k<NCA; k++)
        c[i*NRA+j] += a[i*NRA+j] * b[k*NCA+j];
endTime(time);

/*** Print results ***/
for (i=0; i<NRA; i++)
  {
  for (j=0; j<NCB; j++)
    if(check[i*NRA+j] != c[i*NRA+j]){
        printf("[%d][%d] -->  %f -- %f \n",i,j, check[i*NRA+j], c[i*NRA+j]);
        printf("Test NOT PASSED\n");
        exit(0);
        }
  }
printf ("Test PASSED.\n");

}

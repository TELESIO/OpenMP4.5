#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "Time.h"
#include <math.h>


int main (int argc, char *argv[])
{

    int n = 33554432;

    int *x= (int*)malloc(sizeof(int)*n);           /* matrix A to be multiplied */
    int *check= (int*)malloc(sizeof(int)*n);
    for (int i = 0; i < n; ++i) {
        x[i]=1;
        check[i]=1;
    }

    check[0]=0;
    for (int k = 1; k < n; ++k) {
        check[k]=x[k-1] + check[k-1];
    }


    //Up-Sweep
    int k=0;
#pragma omp target data map(tofrom: x[0:n])
    {
        for (int d = 0; d < (int)(log2(n)); d++) {
            printf("%d \n", d);
#pragma omp target
#pragma omp teams num_teams(65536) thread_limit(512)
#pragma omp distribute parallel for simd
            for (k = ceil(pow(2,d+1)-(1)); k < n; k+=(int)ceil(pow(2,d+1))) {
                x[k] = x[k-(int)ceil(pow(2,d))] + x[k];
            }
        }
    }


#pragma omp target data map(tofrom: x[0:n])
    {
        //Down-Sweep
        x[n-1]=0;
        for (int d = (int)(log2(n))-1; d >= 0 ; d--) {
            printf("%d \n", d);
#pragma omp target
#pragma omp teams thread_limit(d)
#pragma omp distribute parallel for simd
            for (int k = ceil(pow(2,d)-1); k < n; k+=(int)ceil(pow(2,d+1))) {
                int t = x[k];
                x[k] = x[k+(int)ceil(pow(2,d))];
                x[k+(int)ceil(pow(2,d))] += t;

            }
        }
    }

    printf("\n");

    for (int k = 0; k < n; k++)
    {
        if(x[k] != check[k]){
            printf("%d    %d -- %d\n",k, x[k],check[k] );
            printf("Test failed\n");
            exit(0);
        }
    }

    printf("Test passed\n");


    free(x);
    free(check);


    return 0;
}

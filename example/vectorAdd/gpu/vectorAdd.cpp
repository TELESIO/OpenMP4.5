#include <sys/time.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

int main(){


    int numElements = 50000000;
    size_t size = numElements * sizeof(float);
    printf("[Vector addition of %d elements]\n", numElements);

    float *A = (float *)malloc(size);

    float *B = (float *)malloc(size);

    float *C = (float *)malloc(size);
    // Verify that allocations succeeded
    if (A == NULL || B == NULL || C == NULL)
    {
        fprintf(stderr, "Failed to allocate host vectors!\n");
        exit(EXIT_FAILURE);
    }

   // Initialize the host input vectors
    for (int i = 0; i < numElements; ++i)
    {
        A[i] = rand()/(float)RAND_MAX;
        B[i] = rand()/(float)RAND_MAX;
    }
struct Time * time= (struct Time *)malloc(sizeof(struct Time));
startTime(time);
#pragma omp target data map(to: A[0:numElements], B[0:numElements]) map(from: C[0:numElements])
#pragma omp target teams distribute parallel for schedule(static,1)
for(int i=0; i < numElements; i++)
{
	for(int k=0; k<100;k++)
		C[i]= A[i]+B[i];
}


endTime(time);

   // Verify that the result vector is correct
    for (int i = 0; i < numElements; ++i)
    {
        if (fabs(A[i] + B[i] - C[i]) > 1e-5)
        {
            fprintf(stderr, "Result verification failed at element %d!\n", i);
            exit(EXIT_FAILURE);
        }
    }

    printf("Test PASSED\n");
	


}

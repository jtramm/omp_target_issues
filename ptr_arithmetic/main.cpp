#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
  int N = 10;

  // Allocate and initialize array A
  int * A = new int[N];
  for( int i = 0; i < N; i++ )
    A[i] = i;

  // Map A to device
  #pragma omp target enter data map(to: A[:N])

  // Set B to point to last element of A
  int * B = A + N - 1;
  
  // Run device kernel
  #pragma omp target
  {
    B[0] = 20;
  }
  
  // Map A back to host
  #pragma omp target exit data map(from: A[:N])
    
  // Check last element
  assert(A[N-1] == 20 && "Failed correctness check\n");

  printf("Success!\n");

  return 0;
}

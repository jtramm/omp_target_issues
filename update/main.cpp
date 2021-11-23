#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#pragma omp declare target
int a;
#pragma omp end declare target

int main(int argc, char* argv[])
{

  a = 0;

  int iters = 1000;

  double start = omp_get_wtime();
  for(int i = 0; i < iters; i++ )
  {
    #pragma omp target update to(a)

    // JIT
    #pragma omp target
    {
      a += 1;
    }
    
    #pragma omp target update from(a)
    
    a += 1;
  }
  double stop = omp_get_wtime();

  printf("Time per iteration = %.3lf ms\n", 1000 * (stop-start)/iters);

  assert( a == 2 * iters );

  return 0;
}

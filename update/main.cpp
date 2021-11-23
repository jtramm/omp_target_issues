#include <stdio.h>
#include <assert.h>
#include <omp.h>

#pragma omp declare target
int a;
#pragma omp end declare target

void kernel()
{
    #pragma omp target update to(a)

    #pragma omp target
    {
      a += 1;
    }
    
    #pragma omp target update from(a)
    
    a += 1;
}

int main(int argc, char* argv[])
{
  // JIT
  kernel();

  a = 0;
  int iters = 1000;

  double start = omp_get_wtime();
  for(int i = 0; i < iters; i++ )
  {
    kernel();
  }
  double stop = omp_get_wtime();

  printf("Time per iteration = %.3lf us\n", 1e6 * (stop-start)/iters);
  printf("a = %d\n", a);

  assert( a == 2 * iters && "Incorrect Results Detected!" );

  return 0;
}

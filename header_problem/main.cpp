#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>
#include "global.h"
#include "foo.h"
#include "bar.h"

int main(void)
{
  int N = 5;

  // Allocate and initialize host array
  size_t sz = N * sizeof(int);
  global_array = (int *) malloc(sz);
  for( int i = 0; i < N; i++ )
    global_array[i] = i;

  #pragma omp target enter data map(to: global_array[:N])

  #pragma omp target
  {
    foo(N);
  }
  
  #pragma omp target exit data map(from: global_array[:N])
  
  // Return non-zero error code if we failed
  assert( global_array[4] == 8 );

  return 0;
}

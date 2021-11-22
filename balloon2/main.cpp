#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

typedef struct{
  double* arr;
} A;

typedef struct{
  A* arr;
} B;

typedef struct{
  B* arr;
} C;

#pragma omp declare target
C* arr;
#pragma omp end declare target

int main(int argc, char* argv[])
{
  size_t len = 27;
  size_t multi = 80;
  printf("len = %lu total base level data elements = %lu total mapping clauses = %lu\n", len, len*len*len*len*multi, 1 + len*len*len);

  double start, stop, ms;

  // Allocate deep object and set base level arrays to zero
  arr = (C*) malloc(len * sizeof(C));
  for(int i = 0; i < len; i++ )
  {
    arr[i].arr = (B*) malloc(len * sizeof(B));
    for(int j = 0; j < len; j++)
    {
      arr[i].arr[j].arr = (A*) malloc(len * sizeof(A));
      for( int k = 0; k < len; k++ )
      {
        arr[i].arr[j].arr[k].arr = (double*) malloc(len * multi* sizeof(double));

        for( int l = 0; l < len*multi; l++ )
          arr[i].arr[j].arr[k].arr[l] = 0.0;
      }
    }
  }

  // JIT
  #pragma omp target
  {
  }

  // Time empty kernel
  printf("Before data mapping:\n");
  for( int i = 0; i < 5; i++ )
  {
    start = omp_get_wtime();
    #pragma omp target
    {
    }
    stop = omp_get_wtime();
    ms = (stop-start)*1000;
    printf("\tEmpty Kernel Runtime = %.5lf ms\n", ms);
  }

  // Map deep object
  #pragma omp target enter data map(to:arr[:len])
  for(int i = 0; i < len; i++ )
  {
    #pragma omp target enter data map(to:arr[i].arr[:len])
    for(int j = 0; j < len; j++)
    {
      #pragma omp target enter data map(to:arr[i].arr[j].arr[:len])
      for(int k = 0; k < len; k++)
      {
        #pragma omp target enter data map(to:arr[i].arr[j].arr[k].arr[:len*multi])
      }
    }
  }

  // Time empty kernel
  printf("After data mapping:\n");
  for( int i = 0; i < 5; i++ )
  {
    start = omp_get_wtime();
    #pragma omp target
    {
    }
    stop = omp_get_wtime();
    ms = (stop-start)*1000;
    printf("\tEmpty Kernel Runtime = %.5lf ms\n", ms);
  }

  return 0;
}

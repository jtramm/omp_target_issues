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
  printf("len = %lu  total data elements = %lu total mapping clauses = %lu\n", len, len*len*len*len*multi, 1 + len*len*len);
  
  // Allocate array and set to zero
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

  // Map
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

        // If the below region is commented out, things are much faster.
        #pragma omp target
        {
          arr[i].arr[j].arr[k].arr[0] = 1.0;
        }
      }
    }
  }

  int num_tests = 1000;
  double start = omp_get_wtime();

  for( int test = 0; test < num_tests; test++)
    {
    // Run kernel
    #pragma omp target teams distribute parallel for
    for( int i = 0; i < 10; i++)
    {
      //
    }
  }

  double stop = omp_get_wtime();
  printf("Time per kernel = %.3lf ms\n", (stop-start)*1e3/num_tests);

  return 0;
}

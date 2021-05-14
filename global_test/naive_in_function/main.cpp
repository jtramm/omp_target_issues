#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>
#include "global.h"

void print_global(void)
{
  printf("A.a = %lf, A.b = %lf, A.c = \n", A.a, A.b);
  for( int i = 0 ; i < A.N; i++)
    printf("\tA.c[%d] = %lf\n", i, A.c[i]);
}

int main(void)
{
  int N = 5;
  A.a = 1.0;
  A.b = 2.0;
  A.c = new double[N];
  A.N = N;
  for( int i = 0 ; i < N; i++)
    A.c[i] = i;

  print_global();

  #pragma omp target enter data map (to: A, A.c[:N])

  #pragma omp target
  {
    print_global();
  }
    
  print_global();

  return 0;
}

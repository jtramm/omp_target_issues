#include <stdio.h>
#include <omp.h>
#include <math.h>
int main( int argv, char** argc ) {
  int is_initial = omp_is_initial_device();
#pragma omp target map(from:is_initial)
  {
    is_initial = omp_is_initial_device();
  }
  if( !is_initial )
    printf( "Hello world from accelerator.\n" );
  else
    printf( "Hello world from host.\n" );
  return 0;
}

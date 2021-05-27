#include <stdio.h>
#include <stdlib.h>
#include"global.h"

#pragma omp declare target
void foo(int i)
{
  settings::arr[i] *= 2;
} 
#pragma omp end declare target

int main(void)
{
  for( int i = 0; i < len; i++ )
    settings::arr[i] = i;

  #pragma omp target update to(settings::arr)

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < len; i++ )
  {
    foo(i);
  }
  
  #pragma omp target update from(settings::arr)
  
  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6 8 10\n");
  printf("Actual   Result: ");
  for( int i = 0; i < len; i++ )
    printf("%.0lf ", settings::arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( settings::arr[5] != 10 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include"global.h"
#include"transfer.h"

#pragma omp declare target
void foo(int i)
{
  settings::arr[i] *= 2;
} 
#pragma omp end declare target

int main(void)
{
  for( int i = 0; i < 4; i++ )
    settings::arr[i] = i;

  copy_host_to_device();

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < 4; i++ )
  {
    foo(i);
  }
  
  copy_device_to_host();
  
  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6\n");
  printf("Actual   Result: ");
  for( int i = 0; i < 4; i++ )
    printf("%.0lf ", settings::arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( settings::arr[3] != 6 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

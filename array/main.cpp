#include <stdio.h>
#include <stdlib.h>
#include"global.h"
#include"transfer.h"
  
int main(void)
{
  for( int i = 0; i < 4; i++ )
    arr[i] = i;

  copy_host_to_device();

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < 4; i++ )
  {
    arr[i] *= 2;
  }
  
  copy_device_to_host();
  
  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6\n");
  printf("Actual   Result: ");
  for( int i = 0; i < 4; i++ )
    printf("%.0lf ", arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( arr[3] != 6 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

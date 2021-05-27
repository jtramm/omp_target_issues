#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include"global.h"
#include"transfer.h"
  
int main(void)
{
  for( int i = 0; i < 4; i++ )
    arr[i] = i;

  // This works
  //#pragma omp target update to(arr)

  // This does not
  copy_host_to_device();

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < 4; i++ )
  {
    arr[i] *= 2;
  }
  
  // This works
  //#pragma omp target update from(arr)
  
  // This does not
  copy_device_to_host();
  
  assert(arr[3] == 6 && "Correctness check failed");

  return 0;
}

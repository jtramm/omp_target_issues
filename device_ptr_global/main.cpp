#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int * host_arr;

#pragma omp declare target
int * device_arr;
#pragma omp end declare target

int main(void)
{
  int host_id   = omp_get_initial_device();
  int device_id = omp_get_default_device();
  
  int N = 5;

  // Allocate and initialize host array
  size_t sz = N * sizeof(int);
  host_arr = (int *) malloc(sz);
  for( int i = 0; i < N; i++ )
  {
    host_arr[i] = i;
  }

  // Allocate device array and copy data from host -> device
  device_arr = (int *) omp_target_alloc(sz, device_id);
  omp_target_memcpy(device_arr, host_arr, sz, 0, 0, device_id, host_id);

  // Execute device kernel
  #pragma omp target teams distribute parallel for\
  is_device_ptr(device_arr)
  for( int i = 0; i < N; i++)
  {
    device_arr[i] *= 2;
  }
  
  // Copy data from device -> host
  omp_target_memcpy(host_arr, device_arr, sz, 0, 0, host_id, device_id);

  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6 8\n");
  printf("Actual   Result: ");
  for( int i = 0; i < N; i++)
    printf("%d ", host_arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( host_arr[4] != 8 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>

#define USE_DEVICE

void * device_alloc( size_t sz, int device_id )
{
  void * ptr = NULL;

  if( sz > 0 )
  {
    #ifdef USE_DEVICE
    ptr = (void *) omp_target_alloc(sz, device_id);
    #else
    ptr = (void *) malloc(sz);
    #endif
    assert(ptr != NULL);
  }

  return ptr;
}

void device_memcpy( void * dst_ptr, void * src_ptr, size_t sz, int dst_id, int src_id)
{
  #ifdef USE_DEVICE
  omp_target_memcpy(dst_ptr, src_ptr, sz, 0, 0, dst_id, src_id);
  #else
  memcpy(dst_ptr, src_ptr, sz);
  #endif
}

int main(void)
{
  int N = 5;
  int host_id = omp_get_initial_device();
  int device_id = omp_get_default_device();
  size_t sz = N * sizeof(int);

  int * host_arr = (int *) malloc(sz);
  for( int i = 0; i < N; i++ )
  {
    host_arr[i] = i;
  }

  int * device_arr = (int *) device_alloc(sz, device_id);
  device_memcpy(device_arr, host_arr, sz, device_id, host_id);

  #pragma omp target teams distribute parallel for\
  is_device_ptr(device_arr)
  for( int i = 0; i < N; i++)
  {
    device_arr[i] *= 2;
  }
  
  device_memcpy(host_arr, device_arr, sz, host_id, device_id);

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

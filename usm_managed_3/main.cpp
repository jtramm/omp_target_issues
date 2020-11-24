#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct{
  int * host_arr;
} Object;

#pragma omp requires unified_shared_memory
  
//#pragma omp declare target
extern Object obj;
void init_arr(size_t sz);
//#pragma omp end declare target

void * allocator(size_t sz)
{
  void * arr = (void *) omp_target_alloc(sz, omp_get_default_device());
  return arr;
}

int main(void)
{
  int N = 5;
  //int device_id = omp_get_default_device();
  size_t sz = N * sizeof(int);

  init_arr(sz);

  //int * host_arr = (int *) omp_target_alloc(sz, device_id);
  //host_arr = (int *) omp_target_alloc(sz, device_id);
  for( int i = 0; i < N; i++ )
  {
    obj.host_arr[i] = i;
  }

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < N; i++)
  {
    obj.host_arr[i] *= 2;
  }

  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6 8\n");
  printf("Actual   Result: ");
  for( int i = 0; i < N; i++)
    printf("%d ", obj.host_arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( obj.host_arr[4] != 8 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

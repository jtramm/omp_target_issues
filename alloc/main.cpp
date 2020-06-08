#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int host_id;
int device_id;
int n;
  

class MyObject{
  public:
  int * arr_host;
  int * arr_dev;
  int len;
  
  MyObject()
  {
    arr_host = new int[n];
    len = n;
    for( int i = 0; i < n; i++ )
      arr_host[i] = i;
  }

  void allocate_on_device()
  {
    size_t sz = len * sizeof(int);
    arr_dev = (int *) omp_target_alloc(sz, device_id);
  }
  
  void copy_to_device()
  {
    size_t sz = len * sizeof(int);
    omp_target_memcpy(arr_dev, arr_host, sz, 0, 0, device_id, host_id);
  }
  
  void copy_from_device()
  {
    size_t sz = len * sizeof(int);
    omp_target_memcpy(arr_host, arr_dev, sz, 0, 0, host_id, device_id);
  }
};

void show(MyObject * objs, int num_objs)
{
  // Show data on host
  for( int i = 0; i < num_objs; i++)
  {
    printf("Object %d: ", i);
    for( int j = 0; j < objs[i].len; j++)
      printf("%d ", objs[i].arr_host[j]);
    printf("\n");
  }
}

int main(void)
{
  // Set some globals
  host_id = omp_get_initial_device();
  device_id = omp_get_default_device();
  n = 6;

  // Allocate array of objects
  int num_objs = 2;
  MyObject * objs = new MyObject[num_objs];
  
  // Show data on host
  show(objs, num_objs);

  // Allocate on device
  for( int i = 0; i < num_objs; i++)
    objs[i].allocate_on_device();

  // Move data to device
  for( int i = 0; i < num_objs; i++)
    objs[i].copy_to_device();

  // Allocate and copy data to 
  #pragma omp target teams distribute parallel for\
  map(tofrom: objs[0:num_objs])
  for( int i = 0; i < num_objs; i++)
  {
    for( int j = 0; j < objs[i].len; j++ )
      objs[i].arr_dev[j] *= 2;
  }

  // Move data to device
  for( int i = 0; i < num_objs; i++)
    objs[i].copy_from_device();
  
  // Show data on host
  show(objs, num_objs);

  return 0;
}

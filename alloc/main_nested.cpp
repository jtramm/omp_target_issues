#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

class MyObjectB{
  public:
    int * arr_host;
    int * arr_dev;
    int len;
  
    myObjectB()
  {
    arr_host = new int[5];
    len = 5;
  }

  void allocate_on_device()
  {
    int device_id = omp_get_default_device();
    size_t sz = len * sizeof(int);
    arr_dev = (int *) omp_target_alloc(sz, device_id);
  }
  
  void copy_to_device()
  {
    int host_id = omp_get_initial_device();
    int device_id = omp_get_default_device();
    size_t sz = len * sizeof(int);
    omp_target_memcpy(arr_dev, arr_host, sz, 0, 0, device_id, host_id);
  }
  
  void copy_from_device()
  {
    int host_id = omp_get_initial_device();
    int device_id = omp_get_default_device();
    size_t sz = len * sizeof(int);
    omp_target_memcpy(arr_host, arr_dev, sz, 0, 0, host_id, device_id);
  }
};

class MyObjectA{
  public:
  MyObjectB * arr_host;
  myObjectB * arr_dev;
  int len;
  
  myObjectA(int n)
  {
    arr_host = new MyObjectB[n];
    len = n;
  }

  void allocate_on_device()
  {
    int device_id = omp_get_default_device();
    size_t sz = len * sizeof(myObjectB);
    arr_dev = (myObjectB *) omp_target_alloc(sz, device_id);

    for( int i = 0; i < len; i++ )
    {
      sz = arr_host[i].len * sizeof(int);
      arr_host[i].arr_dev = (int *) omp_target_alloc(sz, device_id);
    }

  }
  
  void copy_to_device()
  {
    int host_id = omp_get_initial_device();
    int device_id = omp_get_default_device();
    size_t sz = len * sizeof(myObjectB);
    omp_target_memcpy(arr_dev, arr_host, sz, 0, 0, device_id, host_id);
  }
  
  void copy_from_device()
  {
    int host_id = omp_get_initial_device();
    int device_id = omp_get_default_device();
    size_t sz = len * sizeof(myObjectB);
    omp_target_memcpy(arr_host, arr_dev, sz, 0, 0, host_id, device_id);
  }
};

int main(void)
{
  // Allocate array of objects
  int num_objs = 2;
  MyObjectA * obj = (MyObjectA *) malloc(num_objs * sizeof(MyObjectA));

  // Initialize objects
  for( int i = 0; i < num_objs; i++ )
  {
    obj[i].len = i+2;
    obj[i].arr = (int *) calloc(obj[i].len, sizeof(int));
    for( int j = 0; j < obj[i].len; j++ )
    {
      obj[i].arr[j] = i*j;
    }
  }

  // Allocate and copy data to 

  #pragma omp target teams distribute parallel for\
  map(tofrom: obj)
  for( int i = 0; i < obj.len; i++)
  {
    obj.arr[i] = i;
  }

  // Print expected and actual arrays on host
  printf("Expected Result: 0 1 2 3 4 5\n");
  printf("Actual   Result: ");
  for( int i = 0; i < obj.len; i++)
    printf("%d ", obj.arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( obj.arr[5] != 5 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

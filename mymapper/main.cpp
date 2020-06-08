#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 2

#define RUN_ON_DEVICE

class MyObjectA{
  public:
    MyObjectA()
    {
      data1 = 1;
      data2 = 2;
    }
    void show()
    {
      printf("\t\tObject A Contents:\n");
      printf("\t\t\tdata1 = %d  data2 = %d\n", data1, data2);
    }
    void foo()
    {
      data1++;
      data2++;
    }
    int data1;
    int data2;
};

class MyObjectB{
  public:
    MyObjectB()
    {
      host_arr = new MyObjectA[N];
      len = N;
    }
    void show()
    {
      printf("\tObject B Contents:\n");
      for( int i = 0; i < len; i++ )
      {
        host_arr[i].show();
      }
    }
    void foo()
    {
      for( int i = 0; i < len; i++ )
      {
        #ifdef RUN_ON_DEVICE
        device_arr[i].foo();
        #else
        host_arr[i].foo();
        #endif
      }
    }
    void device_alloc()
    {
      int device_id = omp_get_default_device();
      size_t sz = len * sizeof(MyObjectA);
      device_arr = (MyObjectA *) omp_target_alloc(sz, device_id);
    }
    void copy_to_device()
    {
      int host_id = omp_get_initial_device();
      int device_id = omp_get_default_device();
      size_t sz = len * sizeof(MyObjectA);
      omp_target_memcpy( 
          device_arr,
          host_arr,
          sz,
          0, 
          0, 
          device_id,
          host_id
          );
    }
    void copy_from_device()
    {
      int host_id = omp_get_initial_device();
      int device_id = omp_get_default_device();
      size_t sz = len * sizeof(MyObjectA);
      omp_target_memcpy( 
          host_arr,
          device_arr,
          sz,
          0, 
          0, 
          host_id,
          device_id
          );
    }
    MyObjectA * host_arr;
    MyObjectA * device_arr;
    int len;
};

class MyObjectC{
  public:
    MyObjectC()
    {
      host_arr = new MyObjectB[N];
      len = N;
    }
    void show()
    {
      printf("Object C Contents:\n");
      for( int i = 0; i < len; i++ )
      {
        host_arr[i].show();
      }
    }
    void foo()
    {
      for( int i = 0; i < len; i++ )
      {
        #ifdef RUN_ON_DEVICE
        device_arr[i].foo();
        #else
        host_arr[i].foo();
        #endif
      }
    }
    void device_alloc()
    {
      int device_id = omp_get_default_device();
      size_t sz = len * sizeof(MyObjectB);
      device_arr = (MyObjectB *) omp_target_alloc(sz, device_id);
      for( int i = 0; i < len; i++ )
        host_arr[i].device_alloc();
    }
    void copy_to_device()
    {
      int host_id = omp_get_initial_device();
      int device_id = omp_get_default_device();
      size_t sz = len * sizeof(MyObjectB);
      for( int i = 0; i < len; i++ )
        host_arr[i].copy_to_device();
      omp_target_memcpy( 
          device_arr,
          host_arr,
          sz,
          0, 
          0, 
          device_id,
          host_id
          );
    }
    void copy_from_device()
    {
      int host_id = omp_get_initial_device();
      int device_id = omp_get_default_device();
      size_t sz = len * sizeof(MyObjectB);
      for( int i = 0; i < len; i++ )
        host_arr[i].copy_from_device();
      omp_target_memcpy( 
          host_arr,
          device_arr,
          sz,
          0, 
          0, 
          host_id,
          device_id
          );
    }
    MyObjectB * host_arr;
    MyObjectB * device_arr;
    int len;
};

int main(void)
{
  MyObjectC * outer = new MyObjectC[N];

  for( int i = 0; i < N; i++ )
    outer[i].show();
  
  #ifdef RUN_ON_DEVICE
  for( int i = 0; i < N; i++ )
  {
    outer[i].device_alloc();
    outer[i].copy_to_device();
  }
  #endif

  #ifdef RUN_ON_DEVICE
  #pragma omp target teams distribute parallel for map(tofrom: outer[:N])
  #endif
  for( int i = 0; i < N; i++)
    outer[i].foo();
  
  #ifdef RUN_ON_DEVICE
  for( int i = 0; i < N; i++ )
    outer[i].copy_from_device();
  #endif

  for( int i = 0; i < N; i++ )
    outer[i].show();

  return 0;
}

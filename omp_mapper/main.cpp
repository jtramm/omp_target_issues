#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>

#define N 2

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
      data1 += 10;
      data2 += 20;
    }
    int data1;
    int data2;
};

class MyObjectB{
  public:
    MyObjectB()
    {
      arr = new MyObjectA[N];
      len = N;
    }
    void show()
    {
      printf("\tObject B Contents:\n");
      for( int i = 0; i < len; i++ )
        arr[i].show();
    }
    void foo()
    {
      for( int i = 0; i < len; i++ )
        arr[i].foo();
    }
    MyObjectA * arr;
    int len;
};
#pragma omp declare mapper (MyObjectB obj) map(obj, obj.arr[:obj.len])

class MyObjectC{
  public:
    MyObjectC()
    {
      arr = new MyObjectB[N];
      len = N;
    }
    void show()
    {
      printf("Object C Contents:\n");
      for( int i = 0; i < len; i++ )
        arr[i].show();
    }
    void foo()
    {
      for( int i = 0; i < len; i++ )
        arr[i].foo();
    }
    MyObjectB * arr;
    int len;
};
#pragma omp declare mapper (MyObjectC obj) map(obj, obj.arr[:obj.len])

int main(void)
{
  MyObjectC * outer = new MyObjectC[N];

  printf("Original data hierarchy:\n");
  for( int i = 0; i < N; i++ )
    outer[i].show();

  printf("Sending data to device...\n");
  #pragma omp target enter data map(to:outer[:N])

  printf("Calling foo()...\n");
  #pragma omp target teams distribute parallel for
  for( int i = 0; i < N; i++)
    outer[i].foo();

  printf("foo() complete!\n");
  
  printf("Sending data back to host...\n");
  #pragma omp target exit data map(from:outer[:N])

  printf("Modified Data Hierarchy:\n");
  for( int i = 0; i < N; i++ )
    outer[i].show();

  printf("Testing for correctness...\n");
  assert(outer[1].arr[1].arr[1].data2 == 22);

  return 0;
}

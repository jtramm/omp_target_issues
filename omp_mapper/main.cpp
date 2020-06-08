#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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

  for( int i = 0; i < N; i++ )
    outer[i].show();

  #pragma omp target teams distribute parallel for map(tofrom: outer[:N])
  for( int i = 0; i < N; i++)
    outer[i].foo();

  for( int i = 0; i < N; i++ )
    outer[i].show();

  return 0;
}

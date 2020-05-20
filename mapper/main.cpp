#include <stdio.h>
#include <stdlib.h>

class MyObject{
  public:
    int * arr;
    int len;
};

#pragma omp declare mapper(MyObject a) map(a, a.arr[:a.len])

int main(void)
{
  MyObject obj;
  obj.len = 6;
  obj.arr = (int *) calloc(obj.len, sizeof(int));

  #pragma omp target teams distribute parallel for\
  map(tofrom: obj)
  for( int i = 0; i < obj.len; i++)
  {
    obj.arr[i] = i;
  }

  // Print filled array on host
  printf("Expected Result: 0 1 2 3 4 5\n");
  printf("Actual   Result: ");
  for( int i = 0; i < obj.len; i++)
    printf("%d ", obj.arr[i]);
  printf("\n");
  
  // Return bad code if we failed
  if( obj.arr[5] == 5 )
    return 0;
  else
  {
    printf("Error!\n");
    return 1;
  }
}

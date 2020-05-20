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

  // Print expected and actual arrays on host
  printf("Expected Result: 0 1 2 3 4 5\n");
  printf("Actual   Result: ");
  for( int i = 0; i < obj.len; i++)
    printf("%d ", obj.arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( arr[5] != 5 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

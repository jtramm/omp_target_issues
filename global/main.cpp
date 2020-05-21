#include <stdio.h>
#include <stdlib.h>

#pragma omp declare target
int * arr;
#pragma omp end declare target

#pragma omp declare target
void foo(int i)
{
  arr[i] = i;
} 
#pragma omp end declare target

int main(void)
{
  // Allocate array and set to zero
  int len = 6;
  arr = (int *) calloc( len, sizeof(int) );

  // Map and fill with values on device
  #pragma omp target teams distribute parallel for\
  map(tofrom: arr[:len])
  for( int i = 0; i < len; i++)
  {
    // Calling foo appears to have no effect
    //foo(i);

    // If we uncomment this to inline the contents of foo, it works correctly.
    arr[i] = i;
  }
  
  // Print expected and actual arrays on host
  printf("Expected Result: 0 1 2 3 4 5\n");
  printf("Actual   Result: ");
  for( int i = 0; i < len; i++)
    printf("%d ", arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( arr[5] != 5 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}

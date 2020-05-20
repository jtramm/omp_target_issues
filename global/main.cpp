#include "foo.h"
#include "globals.h"
#include <stdio.h>

int main(void)
{
  // Allocate array
  int len = 6;
  arr = (int *) calloc( len, sizeof(int) );

  // Map and fill with values on device
  #pragma omp target teams distribute parallel for\
  map(tofrom: arr[:len])
  for( int i = 0; i < len; i++)
  {
    // If we call foo, the function appears to have no effect
    foo(i);

    // If we inline the contents of foo, it works correctly.
    //arr[i] = i;
  }

  // Print filled array on host
  for( int i = 0; i < len; i++)
    printf("%d ", arr[i]);
  printf("\n");
  
  // Return bad code if we failed
  if( arr[5] == 5 )
    return 0;
  else
  {
    printf("Error!\n");
    return 1;
  }
}
